#include "Oscillator.h"

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

void Oscillator::UpdatePhase(double dt, double frequency)
{
	phaseIncrement = frequency * dt;
	phase += phaseIncrement;
	while (phase > 1.0) phase -= 1.0;
}

void Oscillator::UpdateWaveformMix(double dt)
{
	if (previousWaveform != waveform && waveformMix[previousWaveform] > 0.0)
	{
		waveformMix[previousWaveform] = lerp(waveformMix[previousWaveform], -.1, 100.0 * dt);
		waveformMix[previousWaveform] = waveformMix[previousWaveform] < 0.0 ? 0.0 : waveformMix[previousWaveform];
	}
	if (waveformMix[waveform] < 1.0)
	{
		waveformMix[waveform] = lerp(waveformMix[waveform], 1.1, 100.0 * dt);
		waveformMix[waveform] = waveformMix[waveform] > 1.0 ? 1.0 : waveformMix[waveform];
	}
}

double Oscillator::Next(double dt, double frequency)
{
	UpdatePhase(dt, frequency);
	UpdateWaveformMix(dt);

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
