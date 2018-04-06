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

double Voice::GetOscillators(double dt)
{
	return GetOsc1(dt);
}

double Voice::Get(double dt)
{
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	return GetOscillators(dt) * volEnv.Get();
}