#pragma once
#include "../types/typedefs.h"
#include <vector>
#include "../types/apidef.h"

namespace perlin
{
	SERVER_API double get(double, double);
}

namespace simplex
{
	SERVER_API const double F2 = (sqrt(3) - 1) / 2;
	SERVER_API const double G2 = (3. - sqrt(3)) / 6;

	SERVER_API double get_orig(double, double);
	SERVER_API double get(double, double);
	SERVER_API double get(double, double, double);
	SERVER_API double get(const std::vector<double>& nDimensionalCoordinates);
	SERVER_API double get(int dimensions, double nDimensionalCoordinates);
}