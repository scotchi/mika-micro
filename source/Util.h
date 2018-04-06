#pragma once

#include <cmath>

const double pi = 2 * acos(0.0);
const double twoPi = 2 * pi;
inline double lerp(double a, double b, double f) { return a + (b - a) * f; }
inline double pitchFactor(double p) { return pow(1.0595, p); }
inline double pitchToFrequency(double p) { return 440.0 * pitchFactor(p - 69); }
inline double fastAtan(double x) { return x / (1.0 + .28 * (x * x)); }