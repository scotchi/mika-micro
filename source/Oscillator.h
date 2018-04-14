#pragma once

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
	void SetSampleRate(double sr) { dt = 1.0 / sr; }
	void SetWaveform(EWaveforms w) { waveform = w; }
	double Next(double frequency);

private:
	double Blep(double phase, double phaseIncrement);

	double dt = 0.0;
	double phase = 0.0;
	EWaveforms waveform = kSine;
};

