#include "Oscillator.h"

void Oscillator::Update(double dt, double frequency)
{
	phaseIncrement = frequency * dt;
	phase += phaseIncrement;
	while (phase > 1.0) phase -= 1.0;
}

double Oscillator::Blep(double phase)
{
	if (phase < phaseIncrement)
	{
		phase /= phaseIncrement;
		return phase + phase - phase * phase - 1.0;
	}
	else if (phase > 1.0 - phaseIncrement)
	{
		phase = (phase - 1.0) / phaseIncrement;
		return phase * phase + phase + phase + 1.0;
	}
	return 0.0;
}

double Oscillator::Get(EWaveforms waveform)
{
	switch (waveform)
	{
	case kSine:
		return sin(phase * twoPi);
	case kSaw:
		return 1.0 - 2.0 * phase + Blep(phase);
	}
}
