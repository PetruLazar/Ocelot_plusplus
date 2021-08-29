#pragma once
#include "apidef.h"

class fixedPoint
{
	int v;

public:
	BASICDEFS_API fixedPoint();
	BASICDEFS_API fixedPoint(int);
	BASICDEFS_API fixedPoint(double);

	BASICDEFS_API operator double();
	void BASICDEFS_API operator=(double);
};

