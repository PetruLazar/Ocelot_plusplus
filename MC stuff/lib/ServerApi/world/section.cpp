#include "section.h"
#include "../types/error.h"
#include "../types/utils.h"

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
void Section::decRefCount(PaletteEntry& bl, const ull& paletteIndex)
{
	bl.referenceCount--;
	if (!bl.referenceCount)
	{
		//remove the entry from the palette and decrement the values that are bigger than this index in the blockStates bit array

		//too expensive, need to figure out an optimization
	}
}
void Section::updateBitsPerBlock()
{
	//if (bitCount(palette.size()))
}
void Section::setBlock(int relX, int relY, int relZ, const BlockState& bl)
{
	//throw runtimeError("Can't you just wait a few days before you try calling this function...?");

	if (useGlobalPallete)
	{
		throw "hmmm... Hmmm!... HMMMM!!!";

		ull blockStatesIndex = ((ull)relY << 8) | ((ull)relZ << 4) | relX,
			oldBlockState = blockStates->getElement(blockStatesIndex);

		if ((int)oldBlockState == (const int&)bl.id)
			//the specified coordinates already contain that block
			return;


		//decrease refCount of old block
		//if (i > oldPaletteEntryIndex) i--;
		//decRefCount(oldPaletteEntry, oldPaletteEntryIndex);

		//increase refCount on new block and update blockStates
		//entry.referenceCount++;
		//blockStates->setElement(blockStatesIndex, i);

		//update blockCount
		//if (oldPaletteEntry.block.id == 0) blockCount++;
		//else if ((int&)bl.id == 0) blockCount--;
		return;

		//block not already present
		//decrease the old blocks refCount
		//decRefCount(oldPaletteEntry, oldPaletteEntryIndex);

		//put the new block in the palette and update blockStates
		palette.push_back(PaletteEntry(bl, 1));
		blockStates->setElement(blockStatesIndex, palette.size() - 1);
		updateBitsPerBlock();
	}
	else
	{
		ull blockStatesIndex = ((ull)relY << 8) | ((ull)relZ << 4) | relX,
			oldPaletteEntryIndex = blockStates->getElement(blockStatesIndex);
		PaletteEntry& oldPaletteEntry = palette[oldPaletteEntryIndex];

		if ((int&)oldPaletteEntry.block.id == (const int&)bl.id)
			//the specified coordinates already contain that block
			return;

		//check if the block is already in the palette
		ull i = 0;
		for (PaletteEntry& entry : palette)
		{
			if ((int&)entry.block.id == (int&)bl.id)//block is already present
			{
				//decrease refCount of old block
				//if (i > oldPaletteEntryIndex) i--;
				decRefCount(oldPaletteEntry, oldPaletteEntryIndex);

				//increase refCount on new block and update blockStates
				entry.referenceCount++;
				blockStates->setElement(blockStatesIndex, i);

				//update blockCount
				if (oldPaletteEntry.block.id == 0) blockCount++;
				else if ((int&)bl.id == 0) blockCount--;
				return;
			}
			i++;
		}

		//block not already present
		//decrease the old blocks refCount
		decRefCount(oldPaletteEntry, oldPaletteEntryIndex);

		//put the new block in the palette and update blockStates
		palette.push_back(PaletteEntry(bl, 1));
		blockStates->setElement(blockStatesIndex, palette.size() - 1);
		updateBitsPerBlock();
	}
}

const varInt LightSection::lightArrayLength = 2048;

LightSection::~LightSection()
{
	if (skyLight) delete skyLight;
	if (blockLight) delete blockLight;
}