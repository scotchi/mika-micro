#include "Voice.h"

double Voice::GetOsc1(double dt)
{
	auto out = 0.0;
	osc1a.Update(dt, baseFrequency * osc1Pitch * osc1SplitFactorA);
	out += osc1a.Get(kSaw);
	if (osc1SplitFactorA != 1.0)
	{
		osc1b.Update(dt, baseFrequency * osc1Pitch * osc1SplitFactorB);
		out += osc1b.Get(kSaw);
	}
	return out;
}

double Voice::GetOsc2(double dt)
{
	auto out = 0.0;
	osc2a.Update(dt, baseFrequency * osc2Pitch * osc2SplitFactorA);
	out += osc2a.Get(kSaw);
	if (osc2SplitFactorA != 1.0)
	{
		osc2b.Update(dt, baseFrequency * osc2Pitch * osc2SplitFactorB);
		out += osc2b.Get(kSaw);
	}
	return out;
}

double Voice::GetOscillators(double dt)
{
	auto out = GetOsc1(dt) * (1.0 - p[kOscMix]) + GetOsc2(dt) * p[kOscMix];
	return out / (1.0 + abs(.5 - p[kOscMix])) * 1.5;
}

double Voice::Get(double dt)
{
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	return GetOscillators(dt) * volEnv.Get();
}