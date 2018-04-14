#include "Voice.h"

void Voice::SetParameter(EParameters parameter, double value)
{
	switch (parameter)
	{
	case kOsc1Wave:
		osc1a.SetWaveform((EWaveforms)(int)value);
		osc1b.SetWaveform((EWaveforms)(int)value);
		break;
	case kOsc1Coarse:
		osc1Coarse = value;
		osc1PitchFactor = pitchFactor(osc1Coarse + osc1Fine);
		break;
	case kOsc1Fine:
		osc1Fine = value;
		osc1PitchFactor = pitchFactor(osc1Coarse + osc1Fine);
		break;
	case kOsc1Split:
		break;
	case kOsc2Wave:
		osc2a.SetWaveform((EWaveforms)(int)value);
		osc2b.SetWaveform((EWaveforms)(int)value);
		break;
	case kOsc2Coarse:
		osc2Coarse = value;
		osc2PitchFactor = pitchFactor(osc2Coarse + osc2Fine);
		break;
	case kOsc2Fine:
		osc2Fine = value;
		osc2PitchFactor = pitchFactor(osc2Coarse + osc2Fine);
		break;
	case kOsc2Split:
		break;
	case kOscMix:
		oscMix = value;
		break;
	case kFmMode:
		break;
	case kFmCoarse:
		break;
	case kFmFine:
		break;
	case kFilterEnabled:
		break;
	case kFilterCutoff:
		break;
	case kFilterResonance:
		break;
	case kFilterKeyTrack:
		break;
	case kVolEnvA:
		volEnv.SetAttack(value);
		break;
	case kVolEnvD:
		volEnv.SetDecay(value);
		break;
	case kVolEnvS:
		volEnv.SetSustain(value);
		break;
	case kVolEnvR:
		volEnv.SetRelease(value);
		break;
	case kVolEnvV:
		break;
	case kModEnvA:
		break;
	case kModEnvD:
		break;
	case kModEnvS:
		break;
	case kModEnvR:
		break;
	case kModEnvV:
		break;
	case kLfoAmount:
		break;
	case kLfoFrequency:
		break;
	case kLfoDelay:
		break;
	case kVolEnvFm:
		break;
	case kVolEnvCutoff:
		break;
	case kModEnvFm:
		break;
	case kModEnvCutoff:
		break;
	case kLfoFm:
		break;
	case kLfoCutoff:
		break;
	case kVoiceMode:
		break;
	case kGlideSpeed:
		break;
	case kMasterVolume:
		break;
	}
}

void Voice::Start() 
{
	if (volEnv.Get() == 0.0)
	{
		oscFm.Reset();
		osc1a.Reset();
		osc1b.Reset();
		osc2a.Reset();
		osc2b.Reset();
	}
	volEnv.stage = kAttack;
}

double Voice::Next()
{
	volEnv.Update();
	auto volEnvValue = volEnv.Get();
	if (volEnvValue == 0.0) return 0.0;

	auto out = 0.0;

	if (oscMix < 1.0)
	{
		auto osc1Out = 0.0;
		osc1a.SetFrequency(baseFrequency * osc1PitchFactor);
		osc1Out += osc1a.Next();
		out += osc1Out * sqrt(1.0 - oscMix);
	}

	if (oscMix > 0.0)
	{
		auto osc2Out = 0.0;
		osc2a.SetFrequency(baseFrequency * osc2PitchFactor);
		osc2Out += osc2a.Next();
		out += osc2Out * sqrt(oscMix);
	}

	out *= volEnv.Get();

	return out;
}
