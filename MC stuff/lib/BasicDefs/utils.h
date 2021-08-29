#pragma once
#include "apidef.h"
#include "../typedefs.h"
#include <string>

BASICDEFS_API std::string to_hexstring(int value);
BASICDEFS_API uint bitCount(ull n);
int BASICDEFS_API fastfloor(double x);