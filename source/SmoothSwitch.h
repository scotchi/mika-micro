#pragma once

enum ESwitchMovement
{
	kIdle,
	kFadeIn,
	kFadeOut
};

enum ESwitchStatus
{
	kOff,
	kMix,
	kOn
};

class SmoothSwitch
{
public:
	ESwitchStatus GetStatus() { return status; }
	double GetValue() { return value; }
	void Switch(bool on);
	void Reset();
	void Update(double dt);

private:
	double value = 0.0;
	ESwitchMovement movement = kIdle;
	ESwitchStatus status = kOff;
};

