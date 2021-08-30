#pragma once

#include "typedefs.h"
#include "apidef.h"

class Angle
{
	byte v;

public:
	
	SERVER_API Angle();
	SERVER_API Angle(byte);
	SERVER_API Angle(double);

	SERVER_API operator double();
	SERVER_API void operator=(double);
};

