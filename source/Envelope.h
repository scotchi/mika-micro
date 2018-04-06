#pragma once

#include "Util.h"

enum EEnvelopeStage
{
	kAttack,
	kDecay,
	kRelease
};

class Envelope
{
public:
	void SetVelocity(double v) { velocity = v; }
	void Reset() { value = 0.0; }
	void Update(double dt, double a, double d, double s, double r);
	double Get(double velocitySensitivity) { return value * (1.0 + velocitySensitivity * (velocity - 1.0)); }

	EEnvelopeStage stage = kRelease;

private:
	double velocity = 0.0;
	double value = 0.0;
};

