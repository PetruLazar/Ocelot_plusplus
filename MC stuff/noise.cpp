#include "noise.h"

namespace perlin
{
	double gradients[16][2] =
	{
		{1,0},
		{0.92387953251128674,0.38268343236508978},
		{0.70710678118654757,0.70710678118654757},
		{0.38268343236508978,0.92387953251128674},
		{0,1},
		{-0.38268343236508978,0.92387953251128674},
		{-0.70710678118654757,0.70710678118654757},
		{-0.92387953251128674,0.38268343236508978},
		{-1,0},
		{-0.92387953251128674,-0.38268343236508978},
		{-0.70710678118654757,-0.70710678118654757},
		{-0.38268343236508978,-0.92387953251128674},
		{0,-1},
		{0.38268343236508978,-0.92387953251128674},
		{0.70710678118654757,-0.70710678118654757},
		{0.70710678118654757,-0.38268343236508978}
	};
	int perm[] =
	{
		6, 4, 12, 7,
		10, 1, 3, 9,
		0, 14, 13, 5,
		15, 8, 2, 11,

		6, 4, 12, 7,
		10, 1, 3, 9,
		0, 14, 13, 5,
		15, 8, 2, 11
	};

	// ratio:
	//0 - completly a
	//1 - completly b
	double dot(double gr[], double x, double y)
	{
		return gr[0] * x + gr[1] * y;
	}
	double mix(double a, double b, double b_ratio)
	{
		return (1 - b_ratio) * a + b_ratio * b;
	}
	//6 * t ^ 5 - 15 * t ^ 4 + 10 * t ^ 3
	double fade(double x)
	{
		return x * x * x * (x * (x * 6 - 15) + 10);
	}

	double get(double x, double y)
	{
		int cellx = fastfloor(x),
			celly = fastfloor(y);

		double relx = x - cellx,
			rely = y - celly;

		cellx &= 0xf;
		celly &= 0xf;

		//select gradient
		int i00 = perm[cellx + perm[celly]],
			i10 = perm[cellx + 1 + perm[celly]],
			i01 = perm[cellx + perm[celly + 1]],
			i11 = perm[cellx + 1 + perm[celly + 1]];

		//extrapolate
		double n00 = dot(gradients[i00], relx, rely),
			n10 = dot(gradients[i10], relx - 1, rely),
			n01 = dot(gradients[i01], relx, rely - 1),
			n11 = dot(gradients[i11], relx - 1, rely - 1);

		//interpolate
		double u = fade(relx),
			v = fade(rely);

		double nx0 = mix(n00, n10, u),
			nx1 = mix(n01, n11, u);

		double nxy = mix(nx0, nx1, v);
		return nxy;
	}
}

double simplex::get(double x, double y)
{
	static const double F = (sqrt(3) - 1) / 2;

	double x_prime = x + (x + y) * F,
		y_prime = y + (x + y) * F;

	double x_base = fastfloor(x_prime),
		y_base = fastfloor(y_prime);

	double x_internal = x_prime - x_base,
		y_internal = y_prime - y_base;

	double x1 = 0, y1 = 0, x2, y2, x3 = 1, y3 = 1;
	if (x_internal > y_internal)
	{
		x2 = 1;
		y2 = 0;
	}
	else
	{
		x2 = 0;
		y2 = 1;
	}

	return 0;
}
double simplex::get(double, double, double)
{
	throw "WIP";
}
double simplex::get(const std::vector<double>& nDimensionalCoordinates)
{
	throw "WIP";
}
double simplex::get(int dimensions, double nDimensionalCoordinates)
{
	throw "WIP";
}