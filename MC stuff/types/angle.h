#pragma once
#include "typedefs.h"

class Angle
{
	byte v;

public:
	
	Angle();
	Angle(byte);
	Angle(double);

	operator double();
	void operator=(double);
};

