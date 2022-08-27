#pragma once
#include "../types/varData.h"
#include "../types/endian.h"
#include "../types/bitarray.h"
#include <vector>
#include "../types/apidef.h"
#include "block.h"
#include "../types/palettedContainer.h"

class Section
{
public:
	SERVER_API Section();
	//SERVER_API Section();
	//SERVER_API ~Section();

	BiomesContainer biomes;
	BlockStatesContainer blockStates;

	bshort blockCount;

	//varInt dataLength; //unused

	//get a copy of the block state at the desired coordinates to use with setBlock
	SERVER_API int getBlock(int relX, int relY, int relZ);
	SERVER_API bool setBlock(int relX, int relY, int relZ, int blockid);
};

class LightSection
{
public:
	SERVER_API ~LightSection();

	SERVER_API static const varInt lightArrayLength;

	BitArray* skyLight,
		* blockLight;
};