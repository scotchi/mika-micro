#pragma once

#include "Util.h"

class Filter
{
public:
	bool IsSilent() { return low == 0.0; }
	void Reset() { reset = true; }
	double Process(double dt, double input, double targetCutoff, double resonance);

private:
	bool reset = false;
	double cutoff = 8000.0;
	double low = 0.0;
	double band = 0.0;
};

