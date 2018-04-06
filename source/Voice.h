#pragma once

#include <array>
#include <cmath>
#include "Envelope.h"
#include "Oscillator.h"
#include "Parameters.h"

class Voice
{
public:
	Voice(std::array<double, kNumParameters> &parameters) : p(parameters) {}

	void SetOsc1Pitch(double p) { osc1Pitch = pitchFactor(p); }
	void SetOsc1Split(double s)
	{
		osc1SplitFactorA = pitchFactor(s);
		osc1SplitFactorB = pitchFactor(s);
	}
	void SetOsc2Pitch(double p) { osc2Pitch = pitchFactor(p); }
	void SetOsc2Split(double s)
	{
		osc2SplitFactorA = pitchFactor(s);
		osc2SplitFactorB = pitchFactor(s);
	}

	void SetNote(int n)
	{
		note = n;
		baseFrequency = pitchToFrequency(n);
	};
	int GetNote() { return note; }
	void Start()
	{
		if (GetVolume() == 0.0)
		{
			osc1a.Reset();
			osc1b.Reset(p[kOsc1Split] < 0.0 ? .33 : 0.0);
			osc2a.Reset();
			osc2b.Reset(p[kOsc2Split] < 0.0 ? .33 : 0.0);
		}
		volEnv.stage = kAttack;
	}
	void Release()
	{
		volEnv.stage = kRelease;
	}
	bool IsReleased() { return volEnv.stage == kRelease; }
	double GetVolume() { return volEnv.Get(); }
	double Get(double dt);

private:
	double GetOsc1(double dt);
	double GetOsc2(double dt);
	double GetOscillators(double dt);

	double osc1Pitch = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	double osc2Pitch = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;

	std::array<double, kNumParameters> &p;
	int note = 69;
	double baseFrequency = 440.0;
	Envelope volEnv;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
};

