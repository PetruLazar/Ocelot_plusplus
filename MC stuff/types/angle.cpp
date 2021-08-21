#include "angle.h"

Angle::Angle() : v(0) { }
Angle::Angle(byte value) : v(value) { }
Angle::Angle(double value) : v(byte(fmod(value, 360) / 360 * 256)) { }

Angle::operator double()
{
	return v * 1.40625;
}
void Angle::operator=(double value)
{
	v = byte((fmod(value, 360) / 360 * 256));
}
