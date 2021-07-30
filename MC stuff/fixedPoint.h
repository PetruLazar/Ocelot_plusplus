#pragma once
class fixedPoint
{
	int v;

public:
	fixedPoint();
	fixedPoint(int);
	fixedPoint(double);

	operator double();
	void operator=(double);
};

