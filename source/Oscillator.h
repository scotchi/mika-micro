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
	void SetFrequency(double f) { frequency = f; }
	double Next();
	void Reset(double p = 0.0) { phase = p; }

private:
	double Blep(double phase, double phaseIncrement);
	double GeneratePulse(double width, double phaseIncrement);

	double dt = 0.0;
	double frequency = 440.0;
	EWaveforms waveform = kSine;
	double phase = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

