#pragma once

#include <cmath>

const double twoPi = 4 * acos(0.0);
inline double lerp(double a, double b, double f) { return a + (b - a) * f; }
inline double pitchFactor(double p) { return pow(1.0595, p); }
inline double pitchToFrequency(double p) { return 440.0 * pitchFactor(p - 69); }