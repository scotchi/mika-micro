#pragma once

#include "Util.h"

enum EEnvelopeStage
{
	kAttack,
	kDecay,
	kRelease
};

struct EnvelopeParameters
{
	double a;
	double d;
	double s;
	double r;
};

class Envelope
{
public:
	void Update(double dt, EnvelopeParameters p);
	double Get() { return value; }

	EEnvelopeStage stage = kAttack;

private:
	double value = 0.0;
};

