#pragma once

#include <cmath>
#include "Util.h"

enum EWaveforms
{
	kSine,
	numWaveforms
};

class Oscillator
{
public:
	void Update(double dt, double frequency);
	double Get(EWaveforms waveform);

private:
	double phase = 0.0;
};

