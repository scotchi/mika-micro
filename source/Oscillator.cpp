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
		break;
	case kSaw:
		return 1.0 - 2.0 * phase + Blep(phase, phaseIncrement);
	case kSquare:
		break;
	case kPulse:
		break;
	case kNoise:
		break;
	case kNumWaveforms:
		break;
	default:
		break;
	}
}
