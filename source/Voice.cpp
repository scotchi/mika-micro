#include "Voice.h"

double Voice::Get(double dt, std::array<double, kNumParameters>& p)
{
	volEnv.Update(dt, p[kVolEnvA], p[kVolEnvD], p[kVolEnvS], p[kVolEnvR]);
	osc.Update(dt, baseFrequency);
	auto out = 0.0;
	out += osc.Get(kSaw) * volEnv.Get();
	return out;
}
