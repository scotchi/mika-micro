#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	// oscillators
	GetParam(kOsc1Wave)->InitEnum("Oscillator 1 waveform", kSaw, kNumWaveforms);
	GetParam(kOsc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24, "semitones");
	GetParam(kOsc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc1Split)->InitDouble("Oscillator 1 split", 0.0, -.025, .025, .01);
	GetParam(kOsc2Wave)->InitEnum("Oscillator 2 waveform", kSaw, kNumWaveforms);
	GetParam(kOsc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24, "semitones");
	GetParam(kOsc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc2Split)->InitDouble("Oscillator 2 split", 0.0, -.025, .025, .01);
	GetParam(kOscMix)->InitDouble("Oscillator mix", 1.0, 0.0, 1.0, .01);

	// fm
	GetParam(kFmMode)->InitEnum("FM mode", 0, 3);
	GetParam(kFmCoarse)->InitInt("FM coarse", 0, 0, 48);
	GetParam(kFmFine)->InitDouble("FM fine", 0.0, -1.0, 1.0, .01);

	// filter
	GetParam(kFilterEnabled)->InitBool("Filter enabled", false);
	GetParam(kFilterCutoff)->InitDouble("Filter cutoff", 8000.0, 20.0, 8000.0, .01, "hz");
	GetParam(kFilterResonance)->InitDouble("Filter resonance", 0.0, 0.0, 1.0, .01);
	GetParam(kFilterKeyTrack)->InitDouble("Filter key tracking", 0.0, -1.0, 1.0, .01);

	// modulation sources
	GetParam(kVolEnvA)->InitDouble("Volume envelope attack", 0.5, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kVolEnvD)->InitDouble("Volume envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kVolEnvS)->InitDouble("Volume envelope sustain", 1.0, 0.0, 1.0, .01);
	GetParam(kVolEnvR)->InitDouble("Volume envelope release", 925.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kVolEnvV)->InitDouble("Volume envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(kModEnvA)->InitDouble("Modulation envelope attack", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kModEnvD)->InitDouble("Modulation envelope decay", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kModEnvS)->InitDouble("Modulation envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(kModEnvR)->InitDouble("Modulation envelope release", 998.0, 0.5, 1000.0, .01, "", "", .025);
	GetParam(kModEnvV)->InitDouble("Modulation envelope velocity sensitivity", 0.0, 0.0, 1.0, .01);
	GetParam(kLfoAmount)->InitDouble("Vibrato amount", 0.0, -0.1, 0.1, .01);
	GetParam(kLfoFrequency)->InitDouble("Vibrato frequency", 4.0, 0.1, 10.0, .01, "", "", 2.0);
	GetParam(kLfoDelay)->InitDouble("Vibrato delay", 0.1, 0.1, 1000.0, .01, "", "", .001);

	// modulation targets
	GetParam(kVolEnvFm)->InitDouble("Volume envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kModEnvFm)->InitDouble("Modulation envelope to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kLfoFm)->InitDouble("Vibrato to FM amount", 0.0, -24.0, 24.0, .01, "semitones");
	GetParam(kVolEnvCutoff)->InitDouble("Volume envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam(kModEnvCutoff)->InitDouble("Modulation envelope to filter cutoff", 0.0, -8000.0, 8000.0, .01, "hz");
	GetParam(kLfoCutoff)->InitDouble("Vibrato to filter cutoff", 0.0, -8000.0, 8000.0, .01);

	// master
	//GetParam(kVoiceMode)->InitEnum("Voice mode", kLegato, kNumVoiceModes);
	GetParam(kGlideSpeed)->InitDouble("Glide speed", 1.0, 1.0, 1000.0, .01, "", "", .1);
	GetParam(kMasterVolume)->InitDouble("Master volume", 0.25, 0.0, 0.5, .01);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParameters, 128, instanceInfo)
{
	TRACE;

	InitParameters();

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 60);

	AttachGraphics(pGraphics);

	MakeDefaultPreset("-", 128);

	voice.SetNote(48);
	voice.Start();
}

MikaMicro::~MikaMicro() {}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		auto out = voice.Next();
		outputs[0][s] = outputs[1][s] = out * .25;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	voice.SetSampleRate(GetSampleRate());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx)
	{
	case kVolEnvA:
	case kVolEnvD:
	case kVolEnvR:
	case kModEnvA:
	case kModEnvD:
	case kModEnvR:
		voice.SetParameter((EParameters)paramIdx, GetParam(paramIdx)->GetMin() + GetParam(paramIdx)->GetMax() - GetParam(paramIdx)->Value());
		break;
	default:
		voice.SetParameter((EParameters)paramIdx, GetParam(paramIdx)->Value());
		break;
	}
	
}
