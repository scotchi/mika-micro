#pragma once

#include "Util.h"

class Filter
{
public:
	void SetSampleRate(double sr) { dt = 1.0 / sr; }
	void SetCutoff(double c) { targetCutoff = c; }
	void SetResonance(double r) { resonance = r; }
	void Reset() { reset = true; }
	double Process(double input);
	bool IsSilent() { return low == 0.0; }

private:
	double dt = 0.0;
	bool reset = false;
	double targetCutoff = 8000.0;
	double cutoff = 8000.0;
	double resonance = 0.0;
	double low = 0.0;
	double band = 0.0;
};

