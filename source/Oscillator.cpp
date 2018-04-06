#include "Oscillator.h"

void Oscillator::Update(double dt, double frequency)
{
	phaseIncrement = frequency * dt;
	phase += phaseIncrement;
	while (phase > 1.0) phase -= 1.0;

	for (int i = 0; i < kNumWaveforms; i++)
	{
		waveformMix[i] = lerp(waveformMix[i], (int)waveform == i ? 1.1 : -.1, 100.0 * dt);
		waveformMix[i] = waveformMix[i] > 1.0 ? 1.0 : waveformMix[i] < 0.0 ? 0.0 : waveformMix[i];
	}
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

double Oscillator::GeneratePulse(double width)
{
	double v = phase < width ? 1.0 : -1.0;
	v += Blep(phase);
	v -= Blep(fmod(phase + (1.0 - width), 1.0));
	return v;
}

double Oscillator::Get()
{
	auto out = 0.0;
	if (waveformMix[kSine] > 0.0)
		out += waveformMix[kSine] * sin(phase * twoPi);
	if (waveformMix[kTriangle] > 0.0)
	{
		triLast = triCurrent;
		triCurrent = phaseIncrement * GeneratePulse(.5) + (1 - phaseIncrement) * triLast;
		out += waveformMix[kTriangle] * triCurrent * 5;
	}
	if (waveformMix[kSaw] > 0.0)
		out += waveformMix[kSaw] * (1.0 - 2.0 * phase + Blep(phase));
	if (waveformMix[kSquare] > 0.0)
		out += waveformMix[kSquare] * GeneratePulse(.5);
	if (waveformMix[kPulse] > 0.0)
		out += waveformMix[kPulse] * GeneratePulse(.75);
	if (waveformMix[kNoise] > 0.0)
	{
		noiseValue += 19.0;
		noiseValue *= noiseValue;
		noiseValue -= (int)noiseValue;
		out += waveformMix[kNoise] * (noiseValue - .5);
	}
	return out;
}
