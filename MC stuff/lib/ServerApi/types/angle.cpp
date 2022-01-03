#include "angle.h"

#include <math.h>

Angle::Angle() : v(0) { }
Angle::Angle(Byte value) : v(value) { }
Angle::Angle(double value) : v(Byte(fmod(value, 360) / 360 * 256)) { }

Angle::operator double()
{
	return v * 1.40625;
}
Angle::operator bfloat()
{
	return v * 1.40625;
}
void Angle::operator=(double value)
{
	v = Byte((fmod(value, 360) / 360 * 256));
}
