#include "Envelope.h"

void Envelope::Update(double dt, EnvelopeParameters p)
{
	switch (stage)
	{
	case kAttack:
		value = lerp(value, 1.1, p.a * dt);
		if (value > 1.0)
		{
			value = 1.0;
			stage = kDecay;
		}
		break;
	case kDecay:
		value = lerp(value, p.s - .1, p.d * dt);
		value = value < p.s ? p.s : value;
		break;
	case kRelease:
		value = lerp(value, -.1, p.r * dt);
		value = value < 0 ? 0 : value;
		break;
	}
}
