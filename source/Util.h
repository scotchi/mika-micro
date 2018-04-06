#pragma once

#include <cmath>

const double twoPi = 4 * acos(0.0);
inline double lerp(double a, double b, double f) { return a + (b - a) * f; }