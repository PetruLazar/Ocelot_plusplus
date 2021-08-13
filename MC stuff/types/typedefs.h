#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <zlib.h>
#include <vector>
#include <SFML/Network.hpp>

typedef unsigned char byte;

typedef unsigned short ush;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long int64;

std::string to_hexstring(int value);
//how many bits are needed to encode an entry with this max (inclusive) value?
uint bitCount(ull n);