#pragma once
#include "../types/varData.h"
#include "../types/endian.h"
#include "../types/bitarray.h"
#include <vector>
#include "../types/apidef.h"
#include "block.h"

class Section
{
	void decRefCount(PaletteEntry& bl, const ull& paletteIndex);
public:
	SERVER_API Section();
	//SERVER_API Section();
	SERVER_API ~Section();

	varInt biomes[4][4][4];

	bshort blockCount;
	Byte bitsPerBlock;

	bool useGlobalPallete;
	std::vector<PaletteEntry> palette;

	varInt dataLength;
	BitArray* blockStates;

	//get direct access to the palette, allowing you to modify all the blocks with the same state in the section
	SERVER_API BlockState& getPaletteEntry(int relX, int relY, int relZ);
	//get direct access to the palette, allowing you to modify all the blocks with the same state in the section
	SERVER_API BlockState& getPaletteEntry(int paletteIndex);
	//get a copy of the block state at the desired coordinates to use with setBlock
	SERVER_API BlockState getBlock(int relX, int relY, int relZ);
	SERVER_API void setBlock(int relX, int relY, int relZ, const BlockState&);
};

class LightSection
{
public:
	SERVER_API ~LightSection();

	SERVER_API static const varInt lightArrayLength;

	BitArray* skyLight,
		* blockLight;
};