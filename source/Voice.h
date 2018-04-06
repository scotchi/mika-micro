#pragma once

#include <array>
#include <cmath>
#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"
#include "Parameters.h"

class Voice
{
public:
	Voice(std::array<double, kNumParameters> &parameters)
		: p(parameters),
		oscFm(kSine),
		osc1a(kSaw),
		osc1b(kSaw),
		osc2a(kSaw),
		osc2b(kSaw)
	{}

	void SetOsc1Wave(EWaveforms w)
	{
		osc1a.SetWaveform(w);
		osc1b.SetWaveform(w);
	}
	void SetOsc1Pitch(double p) { osc1Pitch = pitchFactor(p); }
	void SetOsc1Split(double s)
	{
		osc1SplitFactorA = pitchFactor(s);
		osc1SplitFactorB = pitchFactor(-s);
	}
	void SetOsc2Wave(EWaveforms w)
	{
		osc2a.SetWaveform(w);
		osc2b.SetWaveform(w);
	}
	void SetOsc2Pitch(double p) { osc2Pitch = pitchFactor(p); }
	void SetOsc2Split(double s)
	{
		osc2SplitFactorA = pitchFactor(s);
		osc2SplitFactorB = pitchFactor(-s);
	}
	void SetFilterEnabled(bool e) { filter.SetEnabled(e); }

	void SetNote(int n)
	{
		note = n;
		targetFrequency = pitchToFrequency(n);
	};
	int GetNote() { return note; }
	void ResetPitch() { baseFrequency = targetFrequency; }
	void SetVelocity(double v)
	{
		volEnv.SetVelocity(v);
		modEnv.SetVelocity(v);
		lfoEnv.SetVelocity(v);
	}
	void SetPitchBendFactor(double f) { pitchBendFactor = f; }
	
	void Start();
	void Release() { volEnv.stage = kRelease; }
	bool IsReleased() { return volEnv.stage == kRelease; }
	double GetVolume() { return volEnv.Get(p[kVolEnvV]); }
	double Get(double dt, double lfoValue);

private:
	double GetOscillators(double dt, double lfoValue);
	double GetFilterCutoff(double lfoValue);

	double osc1Pitch = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	double osc1bMix = 0.0;
	double osc2Pitch = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double osc2bMix = 0.0;

	std::array<double, kNumParameters> &p;
	int note = 69;
	double targetFrequency = 440.0;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;
	Envelope volEnv;
	Envelope modEnv;
	Envelope lfoEnv;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Filter filter;
};

