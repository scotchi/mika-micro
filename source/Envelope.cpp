#include "Envelope.h"

void Envelope::Update()
{
	switch (stage)
	{
	case kAttack:
		value += (1.1 - value) * attack * dt;
		if (value >= 1.0)
		{
			value = 1.0;
			stage = kDecay;
		}
		break;
	case kDecay:
		value += (sustain - value) * decay * dt;
		break;
	case kRelease:
		value += (-.1 - value) * release * dt;
		value = value < 0.0 ? 0.0 : value;
		break;
	}
}
