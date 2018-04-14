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
		osc1SplitMix.Switch(value != 0.0);
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
		osc2SplitMix.Switch(value != 0.0);
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
		filterMix.Switch((bool)value);
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
		volEnvVelocitySensitivity = value;
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
		modEnvVelocitySensitivity = value;
		break;
	case kLfoAmount:
		lfoOsc1 = value < 0.0 ? -value : 0.0;
		lfoOsc2 = abs(value);
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
		glideSpeed = value;
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
		osc1SplitMix.Reset();
		osc2SplitMix.Reset();
		filterMix.Reset();
	}
	volEnv.stage = kAttack;
	modEnv.stage = kAttack;
}

double Voice::Next(double lfoValue, double driftValue)
{
	// skip processing if voice is silent
	volEnv.Update();
	auto volEnvValue = volEnv.Get() * (1.0 + (velocity - 1.0) * volEnvVelocitySensitivity);
	if (volEnvValue == 0.0 && filter.IsSilent()) return 0.0;

	// mod envelope
	modEnv.Update();
	auto modEnvValue = modEnv.Get() * (1.0 + (velocity - 1.0) * modEnvVelocitySensitivity);

	// lfo
	lfoDelayMultiplier += (1.0 - lfoDelayMultiplier) * lfoDelay * dt;
	lfoValue *= lfoDelayMultiplier;

	// mono glide
	baseFrequency += (targetFrequency - baseFrequency) * glideSpeed * dt;

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
		osc1SplitMix.Update(dt);
		switch (osc1SplitMix.GetStatus())
		{
		case kMix:
		case kOn:
			osc1b.SetFrequency(osc1Frequency * osc1SplitFactorB);
			switch (osc1SplitMix.GetStatus())
			{
			case kMix:
				osc1Out += osc1b.Next() * osc1SplitMix.GetValue();
				break;
			case kOn:
				osc1Out += osc1b.Next();
				break;
			}
			break;
		}
		out += osc1Out * sqrt(1.0 - oscMix);
	}

	// oscillator 2
	if (oscMix > 0.0)
	{
		auto osc2Out = 0.0;
		osc2a.SetFrequency(osc2Frequency * osc2SplitFactorA);
		osc2Out += osc2a.Next();
		osc2SplitMix.Update(dt);
		switch (osc2SplitMix.GetStatus())
		{
		case kMix:
		case kOn:
			osc2b.SetFrequency(osc2Frequency * osc2SplitFactorB);
			switch (osc2SplitMix.GetStatus())
			{
			case kMix:
				osc2Out += osc2b.Next() * osc2SplitMix.GetValue();
				break;
			case kOn:
				osc2Out += osc2b.Next();
				break;
			}
			break;
		}
		out += osc2Out * sqrt(oscMix);
	}

	// apply volume envelope
	out *= volEnvValue;

	// filter
	filterMix.Update(dt);
	switch (filterMix.GetStatus())
	{
	case kMix:
	case kOn:
		auto cutoff = filterCutoff * (1.0 + driftValue);
		cutoff += filterKeyTracking * baseFrequency * pitchBendFactor;
		cutoff += volEnvCutoff * volEnvValue;
		cutoff += modEnvCutoff * modEnvValue;
		cutoff += lfoCutoff * lfoValue;
		filter.SetCutoff(cutoff);
		switch (filterMix.GetStatus())
		{
		case kMix:
			out = out * (1.0 - filterMix.GetValue()) + filter.Process(out) * filterMix.GetValue();
			break;
		case kOn:
			out = filter.Process(out);
			break;
		}
		break;
	}

	return out;
}
