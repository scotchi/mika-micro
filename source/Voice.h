#pragma once

#include "Envelope.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "Util.h"

class Voice
{
public:
	void SetSampleRate(double sr)
	{
		dt = 1.0 / sr;
		volEnv.SetSampleRate(sr);
		oscFm.SetSampleRate(sr);
		osc1a.SetSampleRate(sr);
		osc1b.SetSampleRate(sr);
		osc2a.SetSampleRate(sr);
		osc2b.SetSampleRate(sr);
	}
	void SetParameter(EParameters parameter, double value);
	void SetNote(int n)
	{
		note = n;
		baseFrequency = pitchToFrequency(note);
	}
	void Start() { volEnv.stage = kAttack; }
	void Release() { volEnv.stage = kRelease; }
	double Next();

private:
	Envelope volEnv;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;

	double dt = 0.0;
	int note = 69;
	double baseFrequency = 440.0;
};

