#include "Oscillator.h"

// http://www.kvraudio.com/forum/viewtopic.php?t=375517
double Oscillator::Blep(double phase, double phaseIncrement)
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


double Oscillator::GeneratePulse(double width, double phaseIncrement)
{
	double v = phase < width ? 1.0 : -1.0;
	v += Blep(phase, phaseIncrement);
	v -= Blep(fmod(phase + (1.0 - width), 1.0), phaseIncrement);
	return v;
}

double Oscillator::Next()
{
	auto phaseIncrement = frequency * dt;
	phase += phaseIncrement;
	while (phase > 1.0) phase -= 1.0;

	switch (waveform)
	{
	case kSine:
		return sin(phase * twoPi);
	case kTriangle:
		triLast = triCurrent;
		triCurrent = phaseIncrement * GeneratePulse(.5, phaseIncrement) + (1.0 - phaseIncrement) * triLast;
		return triCurrent * 5.0;
	case kSaw:
		return 1.0 - 2.0 * phase + Blep(phase, phaseIncrement);
	case kSquare:
		return GeneratePulse(.5, phaseIncrement);
	case kPulse:
		return GeneratePulse(.75, phaseIncrement);
	case kNoise:
		noiseValue += 19.0;
		noiseValue *= noiseValue;
		noiseValue -= (int)noiseValue;
		return noiseValue - .5;
	}
}
