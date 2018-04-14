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
		break;
	case kOsc1Fine:
		break;
	case kOsc1Split:
		break;
	case kOsc2Wave:
		osc2a.SetWaveform((EWaveforms)(int)value);
		osc2b.SetWaveform((EWaveforms)(int)value);
		break;
	case kOsc2Coarse:
		break;
	case kOsc2Fine:
		break;
	case kOsc2Split:
		break;
	case kOscMix:
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

double Voice::Next()
{
	auto out = 0.0;
	osc1a.SetFrequency(baseFrequency);
	out += osc1a.Next();
	volEnv.Update();
	out *= volEnv.Get();
	return out;
}
