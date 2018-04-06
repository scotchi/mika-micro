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
	void Update(double dt, double a, double d, double s, double r);
	double Get() { return value; }

	EEnvelopeStage stage = kRelease;

private:
	double value = 0.0;
};

