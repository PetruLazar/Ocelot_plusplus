#include "angle.h"
#include "basic.h"

Angle::Angle() : v(0) { }
Angle::Angle(byte value) : v(value) { }
Angle::Angle(double value) : v(fmod(value, 360) / 360 * 256) { }
