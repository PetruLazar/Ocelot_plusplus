#pragma once
#include "apidef.h"

class fixedPoint
{
	int v;

public:
	SERVER_API fixedPoint();
	SERVER_API fixedPoint(int);
	SERVER_API fixedPoint(double);

	SERVER_API operator double();
	SERVER_API void operator=(double);
};

