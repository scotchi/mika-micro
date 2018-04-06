#pragma once

#include <cmath>
#include "Util.h"

enum EWaveforms
{
	kSine,
	kSaw,
	numWaveforms
};

class Oscillator
{
public:
	void Reset(double v = 0.0) { t = v; }
	void Update(double dt, double frequency);
	
	double Get(EWaveforms waveform);

private:
	double Blep(double t);

	double t = 0.0;
	double dt = 0.0;
};

