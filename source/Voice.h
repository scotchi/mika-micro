#pragma once

#include "Envelope.h"
#include "Filter.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "SmoothSwitch.h"
#include "Util.h"

enum EVoiceModes
{
	kPoly,
	kMono,
	kLegato,
	kNumVoiceModes
};

class Voice
{
public:
	void SetSampleRate(double sr)
	{
		dt = 1.0 / sr;
		volEnv.SetSampleRate(sr);
		modEnv.SetSampleRate(sr);
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
		targetFrequency = pitchToFrequency(note);
	}
	int GetNote() { return note; }
	void SetVelocity(double v) { velocity = v; }
	void SetPitchBendFactor(double f) { pitchBendFactor = f; }
	void ResetPitch() { baseFrequency = targetFrequency; }
	void Start();
	void Release()
	{
		volEnv.stage = kRelease;
		modEnv.stage = kRelease;
	}
	double Next(double lfoValue, double driftValue);
	double GetVolume() { return volEnv.Get(); }
	bool IsReleased() { return volEnv.stage == kRelease; }

private:
	Envelope volEnv;
	Envelope modEnv;
	Oscillator oscFm;
	Oscillator osc1a;
	Oscillator osc1b;
	Oscillator osc2a;
	Oscillator osc2b;
	Filter filter;

	double dt = 0.0;
	int note = 69;
	double velocity = 0.0;
	double targetFrequency = 440.0;
	double baseFrequency = 440.0;
	double pitchBendFactor = 1.0;

	int osc1Coarse = 0;
	double osc1Fine = 0.0;
	double osc1PitchFactor = 1.0;
	double osc1SplitFactorA = 1.0;
	double osc1SplitFactorB = 1.0;
	SmoothSwitch osc1SplitMix;
	int osc2Coarse = 0;
	double osc2Fine = 0.0;
	double osc2PitchFactor = 1.0;
	double osc2SplitFactorA = 1.0;
	double osc2SplitFactorB = 1.0;
	SmoothSwitch osc2SplitMix;
	double oscMix = 0.0;
	int fmMode = 0;
	double fmCoarse = 0.0;
	double fmFine = 0.0;
	SmoothSwitch filterMix;
	double filterCutoff = 8000.0;
	double filterKeyTracking = 0.0;
	double volEnvVelocitySensitivity = 0.0;
	double modEnvVelocitySensitivity = 0.0;
	double lfoDelay = 0.0;
	double lfoDelayMultiplier = 1.0;
	double volEnvFm = 0.0;
	double volEnvCutoff = 0.0;
	double modEnvFm = 0.0;
	double modEnvCutoff = 0.0;
	double lfoOsc1 = 0.0;
	double lfoOsc2 = 0.0;
	double lfoFm = 0.0;
	double lfoCutoff = 0.0;
	EVoiceModes voiceMode = kLegato;
	double glideSpeed = 0.0;
};

