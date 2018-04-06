#include "Envelope.h"

void Envelope::Update(double dt, double a, double d, double s, double r)
{
	switch (stage)
	{
	case kAttack:
		value = lerp(value, 1.1, a * dt);
		if (value > 1.0)
		{
			value = 1.0;
			stage = kDecay;
		}
		break;
	case kDecay:
		value = lerp(value, s - .1, d * dt);
		value = value < s ? s : value;
		break;
	case kRelease:
		value = lerp(value, -.1, r * dt);
		value = value < 0 ? 0 : value;
		break;
	}
}
