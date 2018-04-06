#pragma once

#include <array>
#include "Envelope.h"
#include "Oscillator.h"
#include "Parameters.h"

class Voice
{
public:
	void SetNote(int n)
	{
		note = n;
		baseFrequency = pitchToFrequency(n);
	};
	void Start()
	{
		volEnv.stage = kAttack;
	}
	double Get(double dt, std::array<double, kNumParameters> &p);

private:
	int note = 69;
	double baseFrequency = 440.0;
	Envelope volEnv;
	Oscillator osc;

};

