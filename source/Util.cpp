#include "Util.h"

#include <climits>

const double pi = 2.0 * acos(0.0);
const double twoPi = 2.0 * pi;

unsigned long xorshiftX = 123456789, xorshiftY = 362436069, xorshiftZ = 521288629;

const double xorshiftMultiplier = 2.0 / ULONG_MAX;
