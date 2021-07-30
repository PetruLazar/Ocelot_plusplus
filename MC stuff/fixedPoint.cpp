#include "fixedPoint.h"
#include "basic.h"

fixedPoint::fixedPoint() : v(0) { }
fixedPoint::fixedPoint(int value) : v(value) { }
fixedPoint::fixedPoint(double d) : v((int)round(d * 32)) { }

fixedPoint::operator double()
{
	return v / 32.;
}
void fixedPoint::operator=(double d)
{
	v = (int)round(d * 32);
}