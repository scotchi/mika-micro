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
		osc1SplitFactorA = 1.0 + value;
		osc1SplitFactorB = 1.0 / osc1SplitFactorA;
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
		osc2SplitFactorA = 1.0 + value;
		osc2SplitFactorB = 1.0 / osc2SplitFactorA;
		break;
	case kOscMix:
		oscMix = value;
		break;
	case kFmMode:
		fmMode = value;
		break;
	case kFmCoarse:
		fmCoarse = value;
		break;
	case kFmFine:
		fmFine = value;
		break;
	case kFilterEnabled:
		filterEnabled = (bool)value;
		break;
	case kFilterCutoff:
		filterCutoff = value;
		break;
	case kFilterResonance:
		filter.SetResonance(value);
		break;
	case kFilterKeyTrack:
		filterKeyTracking = value;
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
		modEnv.SetAttack(value);
		break;
	case kModEnvD:
		modEnv.SetDecay(value);
		break;
	case kModEnvS:
		modEnv.SetSustain(value);
		break;
	case kModEnvR:
		modEnv.SetRelease(value);
		break;
	case kModEnvV:
		break;
	case kLfoAmount:
		if (value < 0.0)
		{
			lfoOsc1 = -value;
			lfoOsc2 = -value;
		}
		else if (value > 0.0)
			lfoOsc2 = value;
		break;
	case kLfoDelay:
		lfoDelay = value;
		break;
	case kVolEnvFm:
		volEnvFm = value;
		break;
	case kVolEnvCutoff:
		volEnvCutoff = value;
		break;
	case kModEnvFm:
		modEnvFm = value;
		break;
	case kModEnvCutoff:
		modEnvCutoff = value;
		break;
	case kLfoFm:
		lfoFm = value;
		break;
	case kLfoCutoff:
		lfoCutoff = copysign((value * .000125) * (value * .000125) * 8000.0, value);
		break;
	case kVoiceMode:
		break;
	case kGlideSpeed:
		break;
	}
}

void Voice::Start() 
{
	if (volEnv.Get() == 0.0)
	{
		modEnv.Reset();
		oscFm.Reset();
		osc1a.Reset();
		osc1b.Reset();
		osc2a.Reset();
		osc2b.Reset();
		filter.Reset();
		lfoDelayMultiplier = 0.0;
	}
	volEnv.stage = kAttack;
	modEnv.stage = kAttack;
}

double Voice::Next(double lfoValue, double driftValue)
{
	// skip processing if voice is silent
	volEnv.Update();
	auto volEnvValue = volEnv.Get();
	if (volEnvValue == 0.0 && filter.IsSilent()) return 0.0;

	// mod envelope
	modEnv.Update();
	auto modEnvValue = modEnv.Get();

	// lfo
	lfoDelayMultiplier += (1.0 - lfoDelayMultiplier) * lfoDelay * dt;
	lfoValue *= lfoDelayMultiplier;

	// oscillator base frequencies
	auto osc1Frequency = baseFrequency * osc1PitchFactor * pitchBendFactor * (1.0 + driftValue);
	osc1Frequency *= 1.0 + lfoOsc1 * lfoValue;
	auto osc2Frequency = baseFrequency * osc2PitchFactor * pitchBendFactor * (1.0 + driftValue);
	osc2Frequency *= 1.0 + lfoOsc2 * lfoValue;

	// fm
	switch (fmMode)
	{
	case 1:
	case 2:
	{
		auto fmAmount = fmCoarse + fmFine;
		fmAmount += volEnvFm * volEnvValue;
		fmAmount += modEnvFm * modEnvValue;
		fmAmount += lfoFm * lfoValue;
		oscFm.SetFrequency(osc1Frequency);
		auto fmValue = pitchFactor(oscFm.Next() * fmAmount);
		switch (fmMode)
		{
		case 1:
			osc1Frequency *= fmValue;
			break;
		case 2:
			osc2Frequency *= fmValue;
			break;
		}
	}
	}

	auto out = 0.0;

	// oscillator 1
	if (oscMix < 1.0)
	{
		auto osc1Out = 0.0;
		osc1a.SetFrequency(osc1Frequency * osc1SplitFactorA);
		osc1Out += osc1a.Next();
		if (osc1SplitFactorA != 1.0)
		{
			osc1b.SetFrequency(osc1Frequency * osc1SplitFactorB);
			osc1Out += osc1b.Next();
		}
		out += osc1Out * sqrt(1.0 - oscMix);
	}

	// oscillator 2
	if (oscMix > 0.0)
	{
		auto osc2Out = 0.0;
		osc2a.SetFrequency(osc2Frequency * osc2SplitFactorA);
		osc2Out += osc2a.Next();
		if (osc2SplitFactorA != 1.0)
		{
			osc2b.SetFrequency(osc2Frequency * osc2SplitFactorB);
			osc2Out += osc2b.Next();
		}
		out += osc2Out * sqrt(oscMix);
	}

	// apply volume envelope
	out *= volEnv.Get();

	// filter
	switch (filterEnabled)
	{
	case true:
		auto cutoff = filterCutoff * (1.0 + driftValue);
		cutoff += filterKeyTracking * baseFrequency * pitchBendFactor;
		cutoff += volEnvCutoff * volEnvValue;
		cutoff += modEnvCutoff * modEnvValue;
		cutoff += lfoCutoff * lfoValue;
		filter.SetCutoff(cutoff);
		out = filter.Process(out);
		break;
	}

	return out;
}
