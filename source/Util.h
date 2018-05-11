#pragma once

#include <cmath>

// math constants //
extern const double pi;
extern const double twoPi;

// pitch calculation //
inline double pitchFactor(double p) { return pow(1.0595, p); }
inline double pitchToFrequency(double p) { return 440.0 * pitchFactor(p - 69); }

// fast trig //
inline double fastAtan(double x) { return x / (1.0 + .28 * (x * x)); }

// random numbers //

// https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c

extern unsigned long xorshiftX, xorshiftY, xorshiftZ;

inline unsigned long xorshift(void)
{
	unsigned long t;
	xorshiftX ^= xorshiftX << 16;
	xorshiftX ^= xorshiftX >> 5;
	xorshiftX ^= xorshiftX << 1;
	t = xorshiftX;
	xorshiftX = xorshiftY;
	xorshiftY = xorshiftZ;
	xorshiftZ = t ^ xorshiftX ^ xorshiftY;
	return xorshiftZ;
}

extern const double xorshiftMultiplier;

inline double fastRandom()
{
	return -1.0 + xorshift() * xorshiftMultiplier;
}
