#include "noise.h"
#include "../types/utils.h"

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

namespace simplex
{
	int perm[] = { 151,160,137,91,90,15,
 131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
 190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
 88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
 77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
 102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
 135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
 5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
 223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
 129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
 251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
 49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
 138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,160,137,91,90,15,
 131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
 190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
 88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
 77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
 102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
 135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
 5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
 223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
 129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
 251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
 49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
 138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
	double grad[12][3] = {
		{1.,1.,0.},
		{-1.,1.,0.},
		{1.,-1.,0.},
		{-1.,-1.,0.},
		{1.,0.,1.},
		{-1.,0.,1.},
		{1.,0.,-1.},
		{-1.,0.,-1.},
		{0.,1.,1.},
		{0.,-1.,1.},
		{0.,1.,-1.},
		{0.,-1.,-1.}
	};

	double dot(double gr[], double x, double y)
	{
		return gr[0] * x + gr[1] * y;
	}
	double dot(double gr[], double x, double y, double z)
	{
		return gr[0] * x + gr[1] * y + gr[2] * z;
	}

	double get(double x, double y)
	{
		double x_prime = x + (x + y) * F2,
			y_prime = y + (x + y) * F2;

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

	double get_orig(double x, double y)
	{
		double n0, n1, n2,
			s;

		s = (x + y) * F2;
		int i = fastfloor(x + s),
			j = fastfloor(y + s);

		// replace with s if unused
		double t = (i + j) * G2;
		double X0 = i - t;
		double Y0 = j - t;
		double x0 = x - X0;
		double y0 = y - Y0;

		int i1, j1;
		if (x0 > y0)
		{
			i1 = 1;
			j1 = 0;
		}
		else
		{
			i1 = 0;
			j1 = 1;
		}

		double x1 = x0 - i1 + G2;
		double y1 = y0 - j1 + G2;
		double x2 = x0 - 1 + 2 * G2;
		double y2 = y0 - 1 + 2 * G2;

		//gradient selection
		int ii = i & 255;
		int jj = j & 255;
		int gi0 = perm[ii + perm[jj]] % 12;
		int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
		int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;

		//extrapolating
		double t0 = .5 - x0 * x0 - y0 * y0;
		if (t0 < 0) n0 = 0.;
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * dot(grad[gi0], x0, y0);
		}

		double t1 = .5 - x1 * x1 - y1 * y1;
		if (t1 < 0) n1 = 0.;
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * dot(grad[gi1], x1, y1);
		}

		double t2 = .5 - x2 * x2 - y2 * y2;
		if (t2 < 0) n2 = 0.;
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot(grad[gi2], x2, y2);
		}

		return (n0 + n1 + n2) * 70;
	}
	double get(double, double, double)
	{
		throw "WIP";
	}
	double get(const std::vector<double>& nDimensionalCoordinates)
	{
		throw "WIP";
	}
	double get(int dimensions, double nDimensionalCoordinates)
	{
		throw "WIP";
	}
}
