#include "Filter.h"

double Filter::Process(double dt, double input, double targetCutoff, double resonance)
{
	// on/off smoothing
	if (mix != enabled ? 1.0 : 0.0)
	{
		mix = lerp(mix, enabled ? 1.1 : -.1, 100.0 * dt);
		mix = mix > 1.0 ? 1.0 : mix < 0.0 ? 0.0 : mix;
	}
	if (mix == 0.0) return input;

	// cutoff clamping
	targetCutoff = targetCutoff > 8000.0 ? 8000.0 : targetCutoff < 20.0 ? 20.0 : targetCutoff;

	// cutoff smoothing
	cutoff = reset ? targetCutoff : lerp(cutoff, targetCutoff, 100.0 * dt);
	reset = false;

	// calculate f
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > 1.0 ? 1.0 : f < .01 ? .01 : f;

	// resonance rolloff
	auto maxResonance = 1.0 - f * f * f * f * f;
	resonance = resonance > maxResonance ? maxResonance : resonance;

	// main processing
	auto high = input - (low + band * (1 - resonance));
	band += f * high;
	low += f * band;
	low = fastAtan(low * .1) * 10.0;

	if (mix == 1.0) return low;
	return input * (1.0 - mix) + low * mix;
}
