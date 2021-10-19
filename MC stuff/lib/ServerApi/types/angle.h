#pragma once

#include "typedefs.h"
#include "apidef.h"

class Angle
{
	Byte v;

public:
	SERVER_API Angle();
	SERVER_API Angle(Byte);
	SERVER_API Angle(double);

	SERVER_API operator double();
	SERVER_API void operator=(double);
};
