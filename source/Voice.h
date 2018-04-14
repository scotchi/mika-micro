#pragma once

#include "Envelope.h"
#include "Filter.h"
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
		filter.SetSampleRate(sr);
	}
	void SetParameter(EParameters parameter, double value);
	void SetNote(int n)
	{
		note = n;
		baseFrequency = pitchToFrequency(note);
	}
	int GetNote() { return note; }
	void Start();
	void Release() { volEnv.stage = kRelease; }
	double Next();
	double GetVolume() { return volEnv.Get(); }
	bool IsReleased() { return volEnv.stage == kRelease; }

private:
	Envelope volEnv;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Filter filter;

	double dt = 0.0;
	int note = 69;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;

	int osc1Coarse = 0;
	double osc1Fine = 0.0;
	double osc1PitchFactor = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	int osc2Coarse = 0;
	double osc2Fine = 0.0;
	double osc2PitchFactor = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	double oscMix = 0.0;
	int fmMode = 0;
	double fmCoarse = 0.0;
	double fmFine = 0.0;
	bool filterEnabled = false;
	double filterCutoff = 8000.0;
	double filterKeyTracking = 0.0;
};

