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
		volEnv.Reset();
		filter.Reset();
	}
	volEnv.stage = kAttack;
}

double Voice::GetOscillators(double dt)
{
	auto osc1Frequency = baseFrequency * osc1Pitch;
	auto osc2Frequency = baseFrequency * osc2Pitch;

	// fm
	auto fmFactor = 1.0;
	if (p[kFmMode] != 0)
	{
		oscFm.Update(dt, osc1Frequency);
		fmFactor = pitchFactor(oscFm.Get(kSine) * (p[kFmCoarse] + p[kFmFine]));
		if (p[kFmMode] == 1) osc1Frequency *= fmFactor;
		if (p[kFmMode] == 2) osc2Frequency *= fmFactor;
	}

	// oscillator 1
	auto osc1Out = 0.0;
	if (p[kOscMix] < 1.0)
	{
		osc1a.Update(dt, osc1Frequency * osc1SplitFactorA);
		osc1Out += osc1a.Get(kSaw);
		if (p[kOsc1Split] != 0.0)
		{
			osc1b.Update(dt, osc1Frequency * osc1SplitFactorB);
			osc1Out += osc1b.Get(kSaw);
		}
	}

	// oscillator 2
	auto osc2Out = 0.0;
	if (p[kOscMix] > 0.0)
	{
		osc2a.Update(dt, osc2Frequency * osc2SplitFactorA);
		osc2Out += osc2a.Get(kSaw);
		if (p[kOsc2Split] != 0.0)
		{
			osc2b.Update(dt, osc2Frequency * osc2SplitFactorB);
			osc2Out += osc2b.Get(kSaw);
		}
	}

	// mix
	auto out = osc1Out * (1.0 - p[kOscMix]) + osc2Out * p[kOscMix];
	return out / (1.0 + abs(.5 - p[kOscMix])) * 1.5;
}

double Voice::Get(double dt)
{
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	if (GetVolume() == 0.0 && filter.IsSilent()) return 0.0;
	auto out = GetOscillators(dt) * volEnv.Get();
	if (p[kFilterEnabled]) out = filter.Process(dt, out, p[kFilterCutoff], p[kFilterResonance]);
	return out;
}