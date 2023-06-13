#pragma once
#include "apidef.h"
#include "typedefs.h"
#include <string>

SERVER_API std::string to_hexstring(int value);
constexpr uint bitCount(ull n)
{
	if (!n) return 1;
	uint count = 0;
	do
	{
		count++;
		n >>= 1;
	} while (n);
	return count;
}
int SERVER_API fastfloor(double x);