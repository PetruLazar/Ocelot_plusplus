#pragma once

#include <vector>
#include "bitarray.h"
#include "varData.h"

struct BasicPaletteEntry
{
	int id;
	int refCount;

	BasicPaletteEntry(int id, int refCount) : id(id), refCount(refCount) {}
};

SERVER_API class PalettedContainer
{
	//Byte bitsPerEntry; //? 0 or min-max or max
	std::vector<BasicPaletteEntry> palette;
	BitArray* values; //values
	const uint size;
	const Byte minBitsPerEntry, maxBitsPerEntry, globalPaletteBitsPerEntry;

	/*void incBitsPerEntry();
	void decBitsPerEntry();
	void incPaletteSize();
	void decPaletteSize();*/

	int getPaletteEntryIndex(int id) const;

public:
	PalettedContainer(uint size,
		Byte minBitsPerEntry,
		Byte maxBitsPerEntry,
		Byte globalPaletteBitsPerEntry,
		int single_value) :
		size(size),
		minBitsPerEntry(minBitsPerEntry),
		maxBitsPerEntry(maxBitsPerEntry),
		globalPaletteBitsPerEntry(globalPaletteBitsPerEntry),
		values(nullptr)
	{
		palette.emplace_back(single_value, size);
	};

	void set(int index, int value);
	int get(int index) const;

	void write(char*&) const;
	void read(char*&);
};