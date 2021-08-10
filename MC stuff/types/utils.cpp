#include "typedefs.h"

std::string to_hexstring(int value)
{
	char res[9] = { 0 }, * cur = res + 7;

	do
	{
		char c = value & 0xf;
		if (c > 9) c += 87; // 87 = 'a' - 10
		else c += '0';
		*(cur--) = c;
		value >>= 4;
	} while (value);

	return std::string(cur + 1);
}