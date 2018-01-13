#pragma once

#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"

class Voice
{
public:
	void SetSampleRate(double sr)
	{
		sampleRate = sr;
		osc1a.SetSampleRate(sr);
		osc1b.SetSampleRate(sr);
		osc2a.SetSampleRate(sr);
		osc2b.SetSampleRate(sr);
	}

	int GetNote() { return note; }
	double GetVolume() { return modEnvelope.Get(); }
	bool IsReleased() { return modEnvelope.IsReleased(); }

	void SetNote(int n);

	void SetOscillator1Split(double s) { oscillator1Split = s; }
	void SetOscillator1Coarse(double c) { oscillator1Coarse = PitchFactor(c); }
	void SetOscillator2Split(double s) { oscillator2Split = s; }
	void SetOscillator2Coarse(double c) { oscillator2Coarse = PitchFactor(c); }
	void SetOscillatorMix(double m) { oscillatorMix = m; }
	void SetEnvelopeAttack(double a) { modEnvelope.SetAttack(a); }
	void SetEnvelopeDecay(double d) { modEnvelope.SetDecay(d); }
	void SetEnvelopeSustain(double s) { modEnvelope.SetSustain(s); }
	void SetEnvelopeRelease(double r) { modEnvelope.SetRelease(r); }
	void SetLfoAmount(double a) { lfoAmount = a; }
	void SetMono(bool m) { mono = m; }
	void SetGlideSpeed(double s) { glideSpeed = s; }

	void Start()
	{
		if (IsReleased())
		{
			frequency = targetFrequency;
		}
		modEnvelope.Start();
	}
	void Release() { modEnvelope.Release(); }

	double Next(double lfoValue);

private:
	double PitchFactor(double p) { return pow(1.05946309436, p); }
	double PitchToFrequency(double p) { return 440.0 * PitchFactor(p - 69); }

	double sampleRate = 44100;
	int note = 69;
	double targetFrequency = 440.0;
	double frequency = 440.0;

	double oscillator1Split = 1.0;
	double oscillator1Coarse = 1.0;
	double oscillator2Split = 1.0;
	double oscillator2Coarse = 1.0;
	double oscillatorMix = 0.0;
	double lfoAmount = 0.0;
	bool mono = false;
	double glideSpeed = 1000.;

	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	Envelope modEnvelope;

	Filter filter;
};
