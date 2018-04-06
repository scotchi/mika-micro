#include "Voice.h"

void Voice::Start()
{
	if (GetVolume() == 0.0)
	{
		oscFm.Reset();
		osc1a.Reset();
		osc1b.Reset(p[kOsc1Split] < 0.0 ? .33 : 0.0);
		osc2a.Reset();
		osc2b.Reset(p[kOsc2Split] < 0.0 ? .33 : 0.0);
		osc1bMix = p[kOsc1Split] != 0.0 ? 1.0 : 0.0;
		osc2bMix = p[kOsc2Split] != 0.0 ? 1.0 : 0.0;
		volEnv.Reset();
		modEnv.Reset();
		lfoEnv.Reset();
		filter.Reset();
	}
	volEnv.stage = kAttack;
	modEnv.stage = kAttack;
	lfoEnv.stage = kAttack;
}

double Voice::GetOscillators(double dt, double lfoValue)
{
	// oscillator frequencies
	auto osc1Frequency = baseFrequency * osc1Pitch * pitchBendFactor;
	if (p[kLfoAmount] < 0.0) osc1Frequency *= 1 + abs(p[kLfoAmount]) * lfoValue;
	auto osc2Frequency = baseFrequency * osc2Pitch * pitchBendFactor;
	if (p[kLfoAmount] != 0.0) osc2Frequency *= 1 + abs(p[kLfoAmount]) * lfoValue;

	// oscillator split smoothing
	osc1bMix = lerp(osc1bMix, (p[kOsc1Split] != 0.0 ? 1.1 : -.1), 100.0 * dt);
	osc1bMix = osc1bMix > 1.0 ? 1.0 : osc1bMix < 0.0 ? 0.0 : osc1bMix;
	osc2bMix = lerp(osc2bMix, (p[kOsc2Split] != 0.0 ? 1.1 : -.1), 100.0 * dt);
	osc2bMix = osc2bMix > 1.0 ? 1.0 : osc2bMix < 0.0 ? 0.0 : osc2bMix;

	// fm
	auto fmFactor = 1.0;
	if (p[kFmMode] != 0)
	{
		oscFm.Update(dt, osc1Frequency);
		auto fmAmount = p[kFmCoarse] + p[kFmFine];
		if (p[kVolEnvFm] != 0.0) fmAmount += volEnv.Get() * p[kVolEnvFm];
		if (p[kModEnvFm] != 0.0) fmAmount += modEnv.Get() * p[kModEnvFm];
		if (p[kLfoFm] != 0.0) fmAmount += lfoValue * p[kLfoFm];
		fmFactor = pitchFactor(oscFm.Get() * fmAmount);
		if (p[kFmMode] == 1) osc1Frequency *= fmFactor;
		if (p[kFmMode] == 2) osc2Frequency *= fmFactor;
	}

	// oscillator 1
	auto osc1Out = 0.0;
	if (p[kOscMix] < 1.0)
	{
		osc1a.Update(dt, osc1Frequency * osc1SplitFactorA);
		osc1Out += osc1a.Get();
		if (osc1bMix > 0.0)
		{
			osc1b.Update(dt, osc1Frequency * osc1SplitFactorB);
			osc1Out += osc1bMix * osc1b.Get();
		}
	}

	// oscillator 2
	auto osc2Out = 0.0;
	if (p[kOscMix] > 0.0)
	{
		osc2a.Update(dt, osc2Frequency * osc2SplitFactorA);
		osc2Out += osc2a.Get();
		if (osc2bMix > 0.0)
		{
			osc2b.Update(dt, osc2Frequency * osc2SplitFactorB);
			osc2Out += osc2bMix * osc2b.Get();
		}
	}

	// mix
	auto out = osc1Out * (1.0 - p[kOscMix]) + osc2Out * p[kOscMix];
	return out / (1.0 + abs(.5 - p[kOscMix])) * 1.5;
}

double Voice::GetFilterCutoff(double lfoValue)
{
	auto cutoff = p[kFilterCutoff];
	if (p[kFilterKeyTrack] != 0.0) cutoff += p[kFilterKeyTrack] * baseFrequency * pitchBendFactor;
	if (p[kVolEnvCutoff] != 0.0) cutoff += volEnv.Get() * p[kVolEnvCutoff];
	if (p[kModEnvCutoff] != 0.0) cutoff += modEnv.Get() * p[kModEnvCutoff];
	if (p[kLfoCutoff] != 0.0)
	{
		auto lfoCutoff = abs(p[kLfoCutoff]);
		lfoCutoff *= .000125;
		lfoCutoff *= lfoCutoff;
		lfoCutoff *= 8000.0;
		if (p[kLfoCutoff] < 0.0) lfoCutoff *= -1;
		cutoff += lfoCutoff * lfoValue;
	}
	return cutoff;
}

double Voice::Get(double dt, double lfoValue)
{
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	if (GetVolume() == 0.0 && filter.IsSilent()) return 0.0;
	modEnv.Update(dt, p[kModEnvA], p[kModEnvD], p[kModEnvS], p[kModEnvR]);
	lfoEnv.Update(dt, p[kLfoDelay], 0.5, 1.0, 0.5);
	lfoValue *= lfoEnv.Get();
	auto out = GetOscillators(dt, lfoValue) * volEnv.Get();
	out = filter.Process(dt, out, GetFilterCutoff(lfoValue), p[kFilterResonance]);
	return out;
}