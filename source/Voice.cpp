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
		filter.Reset();
	}
	volEnv.stage = kAttack;
	modEnv.stage = kAttack;
}

double Voice::GetOscillators(double dt)
{
	// oscillator frequencies
	auto osc1Frequency = baseFrequency * osc1Pitch;
	auto osc2Frequency = baseFrequency * osc2Pitch;

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

double Voice::GetFilterCutoff()
{
	auto cutoff = p[kFilterCutoff];
	if (p[kVolEnvCutoff] != 0.0) cutoff += volEnv.Get() * p[kVolEnvCutoff];
	if (p[kModEnvCutoff] != 0.0) cutoff += modEnv.Get() * p[kModEnvCutoff];
	return cutoff;
}

double Voice::Get(double dt)
{
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	if (GetVolume() == 0.0 && filter.IsSilent()) return 0.0;
	modEnv.Update(dt, p[kModEnvA], p[kModEnvD], p[kModEnvS], p[kModEnvR]);

	auto out = GetOscillators(dt) * volEnv.Get();
	out = filter.Process(dt, out, GetFilterCutoff(), p[kFilterResonance]);
	return out;
}