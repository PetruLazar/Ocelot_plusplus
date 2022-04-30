#pragma once

#include "typedefs.h"
#include "apidef.h"
#include "endian.h"

class Angle
{
	Byte v;

public:
	SERVER_API Angle();
	SERVER_API Angle(Byte);
	SERVER_API Angle(double);
	SERVER_API Angle(float);

	SERVER_API operator float();
	SERVER_API operator bfloat();
	SERVER_API void operator=(double);
};
