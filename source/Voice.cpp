#include "Voice.h"

double Voice::Get(double dt)
{
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	osc.Update(dt, baseFrequency * osc1Pitch);
	auto out = 0.0;
	out += osc.Get(kSaw) * volEnv.Get();
	return out;
}
