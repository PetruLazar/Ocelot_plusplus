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
	int getBlock(int relX, int relY, int relZ) { return blockStates.get(relY << 8 | relZ << 4 | relX); }
	SERVER_API bool setBlock(int relX, int relY, int relZ, int blockid);
};

class LightSection
{
public:
	SERVER_API ~LightSection();

	SERVER_API static const varInt lightArrayLength;

	BitArray* skyLight,
		* blockLight;

	Byte getSkyLight(int relX, int relY, int relZ) { return (Byte)skyLight->getElement((ull)relY << 8 | (ull)relZ << 4 | relX); }
	void setSkyLight(int relX, int relY, int relZ, Byte value) { skyLight->setElement((ull)relY << 8 | (ull)relZ << 4 | relX, value); }
	Byte getBlockLight(int relX, int relY, int relZ) { return (Byte)blockLight->getElement((ull)relY << 8 | (ull)relZ << 4 | relX); }
	void setBlockLight(int relX, int relY, int relZ, Byte value) { blockLight->setElement((ull)relY << 8 | (ull)relZ << 4 | relX, value); }
};