#pragma once

#include <cmath>
#include "Util.h"

enum EWaveforms
{
	kSine,
	kSaw,
	kNumWaveforms
};

class Oscillator
{
public:
	void Reset(double p = 0.0) { phase = p; }
	void Update(double dt, double frequency);
	
	double Get(EWaveforms waveform);

private:
	double Blep(double phase);

	double phase = 0.0;
	double phaseIncrement = 0.0;
};

