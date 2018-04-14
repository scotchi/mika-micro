#include "SmoothSwitch.h"

void SmoothSwitch::Switch(bool on)
{
	switch (on)
	{
	case true:
		movement = kFadeIn;
		status = kMix;
		break;
	case false:
		movement = kFadeOut;
		status = kMix;
		break;
	}
}

void SmoothSwitch::Reset()
{
	switch (movement)
	{
	case kFadeOut:
		value = 0.0;
		movement = kIdle;
		status = kOff;
		break;
	case kFadeIn:
		value = 1.0;
		movement = kIdle;
		status = kOn;
		break;
	}
}

void SmoothSwitch::Update(double dt)
{
	switch (movement)
	{
	case kFadeIn:
		value += 100.0 * dt;
		if (value >= 1.0)
		{
			value = 1.0;
			movement = kIdle;
			status = kOn;
		}
		break;
	case kFadeOut:
		value -= 100.0 * dt;
		if (value <= 0.0)
		{
			value = 0.0;
			movement = kIdle;
			status = kOff;
		}
		break;
	}
}
