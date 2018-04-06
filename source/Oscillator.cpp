#include "Oscillator.h"

void Oscillator::Update(double dt, double frequency)
{
	phase += frequency * dt;
	while (phase > 1) phase -= 1;
}

double Oscillator::Get(EWaveforms waveform)
{
	switch (waveform)
	{
	case kSine:
		return sin(phase * twoPi);
	}
}
