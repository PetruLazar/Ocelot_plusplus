#pragma once
#include <vector>
#include "types/bitarray.h"
#include "types/varData.h"
#include "json.h"

using namespace std;

class OldBlockState
{
	json* state;

public:
	varInt id;

	OldBlockState(int id);
	OldBlockState(const std::string blockName);
	OldBlockState(const std::string blockName, BlockProperty* properties);
	OldBlockState(json* blockState);
	OldBlockState(json& blockState);

	std::string getState(const std::string&) const;
	json* getJsonState() const;
	void setState(const std::string& state, const std::string& value);

	void set(int blockId);
	void set(const std::string& blockName);
	void set(const std::string& blockName, BlockProperty* properties);

	bool operator==(const OldBlockState&) const;
	void operator=(const OldBlockState&);
};

class OldSection
{
	void decRefCount(PaletteEntry& bl, const ull& paletteIndex);
	void updateBitsPerBlock();

	//Byte internalBitsPerBlock;

public:
	OldSection();
	//SERVER_API Section();
	~OldSection();

	//varInt biomes[4][4][4];
	//bits per entry 4, always global palette
	//no palette (uses global)
	BitArray* biomes;

	vector<PaletteEntry> palette;
	bool useGlobalPallete;

	Byte bitsPerBlock;
	bshort blockCount;

	varInt dataLength; //unused
	BitArray* blockStates;

	//get direct access to the palette, allowing you to modify all the blocks with the same state in the section
	OldBlockState& getPaletteEntry(int relX, int relY, int relZ);
	//get direct access to the palette, allowing you to modify all the blocks with the same state in the section
	OldBlockState& getPaletteEntry(int paletteIndex);
	//get a copy of the block state at the desired coordinates to use with setBlock
	OldBlockState getBlock(int relX, int relY, int relZ);
	void setBlock(int relX, int relY, int relZ, const OldBlockState&);
};