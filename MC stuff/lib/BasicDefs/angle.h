#pragma once
#include "../typedefs.h"
#include "apidef.h"

class Angle
{
	byte v;

public:
	
	BASICDEFS_API Angle();
	BASICDEFS_API Angle(byte);
	BASICDEFS_API Angle(double);

	BASICDEFS_API operator double();
	BASICDEFS_API void operator=(double);
};

