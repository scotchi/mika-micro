#include "Oscillator.h"

void Oscillator::Update(double dt, double frequency)
{
	dt = frequency * dt;
	t += dt;
	while (t > 1.0) t -= 1.0;
}

double Oscillator::Blep(double t)
{
	if (t < dt)
	{
		t /= dt;
		return t + t - t * t - 1.0;
	}
	else if (t > 1.0 - dt)
	{
		t = (t - 1.0) / dt;
		return t * t + t + t + 1.0;
	}
	return 0.0;
}

double Oscillator::Get(EWaveforms waveform)
{
	switch (waveform)
	{
	case kSine:
		return sin(t * twoPi);
	case kSaw:
		return 1.0 - 2.0 * t + Blep(t);
	}
}
