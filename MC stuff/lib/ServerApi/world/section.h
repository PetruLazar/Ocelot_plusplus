#pragma once
#include "../types/varData.h"
#include "../types/endian.h"
#include "../types/bitarray.h"
#include <vector>
#include "../types/apidef.h"

class Section
{
public:
	~Section();

	varInt biomes[4][4][4];

	bshort blockCount;
	byte bitsPerBlock;

	bool useGlobalPallete;
	std::vector<varInt> pallete;

	varInt dataLength;
	BitArray* blockStates;
};

class LightSection
{
public:
	~LightSection();

	SERVER_API static const varInt lightArrayLength;

	BitArray* skyLight,
		* blockLight;
};