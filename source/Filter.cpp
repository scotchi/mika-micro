#include "Filter.h"

double Filter::Process(double input)
{
	// cutoff smoothing
	cutoff = reset ? targetCutoff : cutoff + (targetCutoff - cutoff) * 100.0 * dt;
	reset = false;

	// f calculation
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > 1.0 ? 1.0 : f < .01 ? .01 : f;

	// resonance rolloff
	auto maxResonance = 1.0 - f * f * f * f * f;
	auto r = resonance > maxResonance ? maxResonance : resonance;

	// main processing
	auto high = input - (low + band * (1.0 - r));
	band += f * high;
	low += f * band;
	low = fastAtan(low * .1) * 10.0;

	return low;
}
