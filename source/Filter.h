#pragma once

#include "Util.h"

class Filter
{
public:
	void SetEnabled(bool e) { enabled = e; }
	bool IsSilent() { return low == 0.0; }
	void Reset()
	{
		reset = true;
		mix = enabled ? 1.0 : 0.0;
	}
	double Process(double dt, double input, double targetCutoff, double resonance);

private:
	bool enabled = false;
	double mix = 0.0;
	bool reset = false;
	double cutoff = 8000.0;
	double low = 0.0;
	double band = 0.0;
};

