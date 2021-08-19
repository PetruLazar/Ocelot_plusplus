#pragma once
#include "types/typedefs.h"

namespace perlin
{
	double get(double, double);
}

namespace simplex
{
	const double F2 = (sqrt(3) - 1) / 2;
	const double G2 = (3. - sqrt(3)) / 6;

	double get_orig(double, double);
	double get(double, double);
	double get(double, double, double);
	double get(const std::vector<double>& nDimensionalCoordinates);
	double get(int dimensions, double nDimensionalCoordinates);
}