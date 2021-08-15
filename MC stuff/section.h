#pragma once
#include "types.h"

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