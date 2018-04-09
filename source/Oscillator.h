#pragma once

#include <array>
#include <cmath>
#include "Util.h"

enum EWaveforms
{
	kSine,
	kTriangle,
	kSaw,
	kSquare,
	kPulse,
	kNoise,
	kNumWaveforms
};

class Oscillator
{
public:
	Oscillator(EWaveforms w) : waveform(w) {}

	void SetWaveform(EWaveforms w)
	{
		previousWaveform = waveform;
		waveform = w;
	}
	void Reset(double p = 0.0)
	{
		phase = p;
		for (int i = 0; i < kNumWaveforms; i++)
			waveformMix[i] = (int)waveform == i ? 1.0 : 0.0;
	}
	double Next(double dt, double frequency);

private:
	double Blep(double phase);
	double GeneratePulse(double width);
	void UpdatePhase(double dt, double frequency);
	void UpdateWaveformMix(double dt);

	EWaveforms waveform = kSaw;
	EWaveforms previousWaveform = kSaw;
	std::array<double, kNumWaveforms> waveformMix;
	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

