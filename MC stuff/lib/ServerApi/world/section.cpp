#include "section.h"
#include "../types/error.h"

Section::Section() { }
Section::~Section()
{
	delete blockStates;
}

BlockState& Section::getPaletteEntry(int relX, int relY, int relZ)
{
	return palette[blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX)].block;
}
BlockState& Section::getPaletteEntry(int paletteIndex)
{
	return palette[paletteIndex].block;
}
BlockState Section::getBlock(int relX, int relY, int relZ)
{
	return palette[blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX)].block;
}
void Section::setBlock(int relX, int relY, int relZ, const BlockState& bl)
{
	throw runtimeError("Can't you just wait a few days before you try calling this function...?");

	//WIP
	PaletteEntry& oldPaletteEntry = palette[blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX)];

	if ((int&)oldPaletteEntry.block.id == (const int&)bl.id)
		//the specified coordinates already contain that block
		return;
	ull i = 0;
	for (PaletteEntry& entry : palette)
	{
		if (entry.block == bl)
		{
			//block is already present
			//oldPaletteEntry.ref
			entry.referenceCount++;
			blockStates->setElement(((ull)relY << 8) | ((ull)relZ << 4) | relX, i);
			return;
		}
		i++;
	}
	//block not already present
	//decrease the old blocks refCount
	oldPaletteEntry.referenceCount--;
	if (!oldPaletteEntry.referenceCount)
	{
		//remove the entry from the palette and decrement the values that are bigger than this index in the blockStates bit array

		//too expensive, need to figure out an optimization
	}
	//palette.push_back();
}

const varInt LightSection::lightArrayLength = 2048;

LightSection::~LightSection()
{
	if (skyLight) delete skyLight;
	if (blockLight) delete blockLight;
}