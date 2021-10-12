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
	if (useGlobalPallete)
	{
		int idToSearch = blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX);
		for (PaletteEntry& entry : palette) if (entry.block.id == idToSearch) return entry.block;
		throw 0; // id not found
	}
	return palette[blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX)].block;
}
BlockState& Section::getPaletteEntry(int paletteIndex)
{
	return palette[paletteIndex].block;
}
BlockState Section::getBlock(int relX, int relY, int relZ)
{
	if (useGlobalPallete)
	{
		return Registry::getBlockState(blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX));
	}
	return palette[blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX)].block;
}
void Section::decRefCount(PaletteEntry& bl, const ull& paletteIndex)
{
	bl.referenceCount--;
	if (!bl.referenceCount)
	{
		//remove the entry from the palette and decrement the values that are bigger than this index in the blockStates bit array
		palette.erase(palette.begin() + paletteIndex);
		for (ull i = 0; i < 4096; i++)
		{
			ull value = blockStates->getElement(i);
			if (value > paletteIndex) blockStates->setElement(i, value - 1);
		}

		//update bits per block
		updateBitsPerBlock();

		//expensive, need to figure out an optimization
	}
}
void Section::updateBitsPerBlock()
{
	//palette cannot be empty
	uint newBitsPerBlock = bitCount(palette.size() - 1);
	if (newBitsPerBlock >= 4 && newBitsPerBlock != bitsPerBlock)
	{
		if (newBitsPerBlock <= 8)
		{
			//local palette
			blockStates->changeBitsPerEntry(newBitsPerBlock);
			bitsPerBlock = newBitsPerBlock;
		}
		else
		{
			throw "Na";
			//move section to global palette
		}
	}

}
void Section::setBlock(int relX, int relY, int relZ, const BlockState& bl)
{
	//throw runtimeError("Can't you just wait a few days before you try calling this function...?");

	//modify:
	// 	   blockStates
	// 	   palette
	// 	   blockCount
	// 
	// 	   bitsPerBlock
	// 	   useGlobalPalette

	ull blockStatesIndex = ((ull)relY << 8) | ((ull)relZ << 4) | relX,
		blockStatesValue = blockStates->getElement(blockStatesIndex);

	//new
	if (useGlobalPallete)
	{
		throw 0;
	}
	else
	{
		PaletteEntry& oldPaletteEntry = palette[blockStatesValue];

		if ((int&)oldPaletteEntry.block.id == (const int&)bl.id)
			//the specified coordinates already contain the specified block
			return;

		//check if the block is already in the palette
		ull i = 0;
		for (PaletteEntry& entry : palette)
		{
			if ((int&)entry.block.id == (const int&)bl.id)
			{
				//block found in the palette
				if ((int&)oldPaletteEntry.block.id == 0) blockCount++;
				else if ((int&)bl.id == 0) blockCount--;
				blockStates->setElement(blockStatesIndex, i);

				//take care of the new block
				entry.referenceCount++;

				//take care of the previous block
				//update old refCount and bitsPerBlock if necessary
				decRefCount(oldPaletteEntry, i);

				return;
			}
			i++;
		}

		//block not present in palette
		if ((int&)oldPaletteEntry.block.id == 0) blockCount++;
		else if ((int&)bl.id == 0) blockCount--;

		//test the refCount for the old block
		if (oldPaletteEntry.referenceCount == 1)
		{
			//put the new block in the old entry
			oldPaletteEntry.block.set((int&)bl.id);
		}
		else
		{
			//create a different palette entry for the new block
			oldPaletteEntry.referenceCount--;
			palette.emplace_back(bl, 1);
			ull paletteSize = palette.size() - 1;
			blockStates->setElement(blockStatesIndex, paletteSize);
			updateBitsPerBlock();
		}
	}
}

const varInt LightSection::lightArrayLength = 2048;

LightSection::~LightSection()
{
	if (skyLight) delete skyLight;
	if (blockLight) delete blockLight;
}