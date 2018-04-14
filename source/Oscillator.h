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
	kNumWaveforms,
	kNone
};

class Oscillator
{
public:
	void SetSampleRate(double sr) { dt = 1.0 / sr; }
	void SetWaveform(EWaveforms w)
	{
		previousWaveform = waveform;
		waveform = w;
		crossfading = true;
		waveformMix = 0.0;
	}
	void SetFrequency(double f) { frequency = f; }
	double Next();
	void Reset(double p = 0.0)
	{
		phase = p;
		crossfading = false;
		waveformMix = 1.0;
	}

private:
	double Blep(double phase);
	double GeneratePulse(double width);
	double Get(EWaveforms waveform);

	double dt = 0.0;
	double frequency = 440.0;
	EWaveforms previousWaveform = kNone;
	EWaveforms waveform = kSine;
	double waveformMix = 1.0;
	bool crossfading = false;
	double phase = 0.0;
	double phaseIncrement = 0.0;
	double triCurrent = 0.0;
	double triLast = 0.0;
	double noiseValue = 19.1919191919191919191919191919191919191919;
};

