#pragma once
#include "apidef.h"
#include "typedefs.h"
#include <string>

SERVER_API std::string to_hexstring(int value);
SERVER_API uint bitCount(ull n);
int SERVER_API fastfloor(double x);