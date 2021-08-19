#pragma once
#include "types/typedefs.h"

namespace perlin
{
	double get(double, double);
}

namespace simplex
{
	double get(double, double);
	double get(double, double, double);
	double get(const std::vector<double>& nDimensionalCoordinates);
	double get(int dimensions, double nDimensionalCoordinates);
}