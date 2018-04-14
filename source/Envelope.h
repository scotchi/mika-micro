#pragma once

enum EEnvelopeStages
{
	kAttack,
	kDecay,
	kRelease
};

class Envelope
{
public:
	void SetSampleRate(double sr) { dt = 1.0 / sr; }
	void SetAttack(double a) { attack = a; }
	void SetDecay(double d) { decay = d; }
	void SetSustain(double s) { sustain = s; }
	void SetRelease(double r) { release = r; }

	void Update();
	double Get() { return value; }

	EEnvelopeStages stage = kRelease;

private:
	double dt = 0.0;
	double attack = 1.0;
	double decay = 1.0;
	double sustain = 1.0;
	double release = 1.0;

	double value = 0.0;
};

