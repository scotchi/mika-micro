#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	GetParam(kOsc1Coarse)->InitInt("Oscillator 1 coarse", 0, -24, 24);
	GetParam(kOsc1Fine)->InitDouble("Oscillator 1 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc1Split)->InitDouble("Oscillator 1 split", 0.0, -.5, .5, .01, "semitones");
	GetParam(kOsc2Coarse)->InitInt("Oscillator 2 coarse", 0, -24, 24);
	GetParam(kOsc2Fine)->InitDouble("Oscillator 2 fine", 0.0, -1.0, 1.0, .01, "semitones");
	GetParam(kOsc2Split)->InitDouble("Oscillator 2 split", 0.0, -.5, .5, .01, "semitones");
	GetParam(kOscMix)->InitDouble("Oscillator mix", 0.0, 0.0, 1.0, .01);

	GetParam(kVolEnvA)->InitDouble("Volume envelope attack", 1000.0, 0.1, 1000.0, .01);
	GetParam(kVolEnvD)->InitDouble("Volume envelope decay", 1.0, 0.1, 1000.0, .01);
	GetParam(kVolEnvS)->InitDouble("Volume envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(kVolEnvR)->InitDouble("Volume envelope release", 1000.0, 0.1, 1000.0, .01);
}

void MikaMicro::InitGraphics()
{
	pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knobLeft = pGraphics->LoadIBitmap(KNOBLEFT_ID, KNOBLEFT_FN, 100);
	auto knobMiddle = pGraphics->LoadIBitmap(KNOBMIDDLE_ID, KNOBMIDDLE_FN, 100);
	auto knobRight = pGraphics->LoadIBitmap(KNOBRIGHT_ID, KNOBRIGHT_FN, 100);
	auto slider = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN, 1);
	auto sliderBg = pGraphics->LoadIBitmap(SLIDERBG_ID, SLIDERBG_FN, 1);
	auto waveformSwitch = pGraphics->LoadIBitmap(WAVEFORMSWITCH_ID, WAVEFORMSWITCH_FN, kNumWaveforms);
	auto toggleSwitch = pGraphics->LoadIBitmap(TOGGLESWITCH_ID, TOGGLESWITCH_FN, 2);
	auto fmModeSwitch = pGraphics->LoadIBitmap(FMMODESWITCH_ID, FMMODESWITCH_FN, 3);

	// oscillators
	//pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 10 * 4, kOsc1Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 10 * 4, kOsc1Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 10 * 4, kOsc1Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 10 * 4, kOsc1Split, &knobMiddle));
	//pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 26 * 4, kOsc2Wave, &waveformSwitch));
	pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 26 * 4, kOsc2Coarse, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 26 * 4, kOsc2Fine, &knobMiddle));
	pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 26 * 4, kOsc2Split, &knobMiddle));
	pGraphics->AttachControl(new IBitmapControl(this, 91.5 * 4, 15 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 90.5 * 4, 16 * 4, 20 * 4, kOscMix, &slider));

	// fm
	//pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 42 * 4, kFmMode, &fmModeSwitch));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 42 * 4, kFmCoarse, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 42 * 4, kFmFine, &knobMiddle));

	// filter
	//pGraphics->AttachControl(new ISwitchControl(this, 22 * 4, 62 * 4, kFilterEnabled, &toggleSwitch));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 62 * 4, kFilterCutoff, &knobRight));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 54 * 4, 62 * 4, kFilterResonance, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 70 * 4, 62 * 4, kFilterKeyTrack, &knobMiddle));

	// modulation sources
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 23 * 4, 20 * 4, kVolEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 23 * 4, 20 * 4, kVolEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 23 * 4, 20 * 4, kVolEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 22 * 4, &sliderBg));
	pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 23 * 4, 20 * 4, kVolEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 22 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 23 * 4, 20 * 4, kVolEnvV, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 121.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 120.5 * 4, 57.5 * 4, 20 * 4, kModEnvA, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 129.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 128.5 * 4, 57.5 * 4, 20 * 4, kModEnvD, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 137.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 136.5 * 4, 57.5 * 4, 20 * 4, kModEnvS, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 145.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 144.5 * 4, 57.5 * 4, 20 * 4, kModEnvR, &slider));
	pGraphics->AttachControl(new IBitmapControl(this, 153.5 * 4, 56.5 * 4, &sliderBg));
	//pGraphics->AttachControl(new IFaderControl(this, 152.5 * 4, 57.5 * 4, 20 * 4, kModEnvV, &slider));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 13.5 * 4, kLfoAmount, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 13.5 * 4, kLfoFrequency, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 13.5 * 4, kLfoDelay, &knobLeft));

	// targets
	//pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 50.5 * 4, kVolEnvFm, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 50.5 * 4, kModEnvFm, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 50.5 * 4, kLfoFm, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 171 * 4, 66.5 * 4, kVolEnvCutoff, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 187 * 4, 66.5 * 4, kModEnvCutoff, &knobMiddle));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 203 * 4, 66.5 * 4, kLfoCutoff, &knobMiddle));

	// master
	//pGraphics->AttachControl(new ISwitchControl(this, 6 * 4, 90 * 4, kVoiceMode, &fmModeSwitch));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 22 * 4, 90 * 4, kGlideSpeed, &knobLeft));
	//pGraphics->AttachControl(new IKnobMultiControl(this, 38 * 4, 90 * 4, kMasterVolume, &knobLeft));

	//pGraphics->AttachControl(new PresetMenu(this, IRECT(0, 0, 100, 25)));

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	for (int i = 0; i < 8; i++) voices.push_back(Voice(parameters));
	MakeDefaultPreset("-", 1);
}

void MikaMicro::FlushMidi(int sample)
{
	while (!midiQueue.Empty())
	{
		auto *message = midiQueue.Peek();
		if (message->mOffset > sample) break;

		if (message->StatusMsg() == IMidiMsg::kNoteOff || (message->StatusMsg() == IMidiMsg::kNoteOn && message->Velocity() == 0))
		{
			for (auto &voice : voices)
				if (voice.GetNote() == message->NoteNumber()) voice.Release();
		}
		else if (message->StatusMsg() == IMidiMsg::kNoteOn)
		{
			auto voice = std::min_element(std::begin(voices), std::end(voices), [](Voice a, Voice b) {
				return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
			});
			voice->SetNote(message->NoteNumber());
			voice->Start();
		}

		midiQueue.Remove();
	}
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		auto out = 0.0;
		for (auto &voice : voices) out += voice.Get(dt) * .25;
		outputs[0][s] = outputs[1][s] = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	dt = 1.0 / GetSampleRate();
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	parameters[paramIdx] = GetParam(paramIdx)->Value();

	switch (paramIdx)
	{
	case kOsc1Coarse:
	case kOsc1Fine:
		for (auto &voice : voices) voice.SetOsc1Pitch(parameters[kOsc1Coarse] + parameters[kOsc1Fine]);
		break;
	case kOsc1Split:
		for (auto &voice : voices) voice.SetOsc1Split(parameters[kOsc1Split]);
		break;
	case kOsc2Coarse:
	case kOsc2Fine:
		for (auto &voice : voices) voice.SetOsc2Pitch(parameters[kOsc2Coarse] + parameters[kOsc2Fine]);
		break;
	case kOsc2Split:
		for (auto &voice : voices) voice.SetOsc2Split(parameters[kOsc2Split]);
		break;
	}
}
