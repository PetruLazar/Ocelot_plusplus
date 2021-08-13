#pragma once
#include "types.h"

class Section
{
	varInt biomes[64];

	bshort blockCount;
	byte bitsPerBlock;

	bool indirectPallete;
	std::vector<varInt> pallete;

	varInt dataLength;
	//bitstream data
public:

};

