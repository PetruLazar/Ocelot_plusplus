#include "section.h"
#include "../debug/mcexceptions.h"
#include "../types/utils.h"

Section::Section() { }
Section::~Section()
{
	delete blockStates;
	delete biomes;
}

BlockState& Section::getPaletteEntry(int relX, int relY, int relZ)
{
	if (useGlobalPallete)
	{
		int idToSearch = (int)blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX);
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
		return Registry::getBlockState((int)blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX));
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
		if (!useGlobalPallete)
		{
			for (ull i = 0; i < 4096; i++)
			{
				ull value = blockStates->getElement(i);
				if (value > paletteIndex) blockStates->setElement(i, value - 1);
			}
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

	if (useGlobalPallete)
	{
		if (newBitsPerBlock != bitsPerBlock && newBitsPerBlock <= 8)
		{
			//move section to local palette
			if (newBitsPerBlock < 4) bitsPerBlock = 4;
			else bitsPerBlock = newBitsPerBlock;
			for (ull i = 0; i < 4096; i++)
			{
				ull blockStatesValue = blockStates->getElement(i);
				ull j = 0;
				for (PaletteEntry& entry : palette)
				{
					if (entry.block.id == blockStatesValue)
					{
						blockStates->setElement(i, j);
						break;
					}
					j++;
				}
			}
			blockStates->changeBitsPerEntry(bitsPerBlock);

			useGlobalPallete = false;
		}
	}
	else
	{
		if (newBitsPerBlock != bitsPerBlock && newBitsPerBlock >= 4)
		{
			if (newBitsPerBlock <= 8)
			{
				//local palette
				bitsPerBlock = newBitsPerBlock;
				blockStates->changeBitsPerEntry(bitsPerBlock);
			}
			else
			{
				//throw "Na";
				//move section to global palette
				useGlobalPallete = true;
				bitsPerBlock = bitCount(Registry::getBlockStateCount());
				blockStates->changeBitsPerEntry(bitsPerBlock);
				for (ull i = 0; i < 4096; i++)
				{
					ull val = blockStates->getElement(i);
					PaletteEntry& entry = palette[val];
					ull val2 = (int)entry.block.id;
					blockStates->setElement(i, val2);
				}
			}
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
		if (blockStatesValue == (const int&)bl.id)
			//the specified coordinates already contain the specified block
			return;

		PaletteEntry* oldPaletteEntry = nullptr;
		ull oldPaletteEntryIndex = 0;
		for (PaletteEntry& entry : palette)
		{
			if ((int&)entry.block.id == blockStatesValue)
			{
				oldPaletteEntry = &entry;
				break;
			}
			oldPaletteEntryIndex++;
		}

		//check if the block is already in the palette
		ull i = 0;
		for (PaletteEntry& entry : palette)
		{
			if ((int&)entry.block.id == (const int&)bl.id)
			{
				//block found in the palette
				if ((int&)oldPaletteEntry->block.id == 0) blockCount++;
				else if ((int&)bl.id == 0) blockCount--;
				blockStates->setElement(blockStatesIndex, (const int&)bl.id);

				//take care of the new block
				entry.referenceCount++;

				//take care of the previous block
				//update old refCount and bitsPerBlock if necessary
				decRefCount(*oldPaletteEntry, oldPaletteEntryIndex);

				return;
			}
			i++;
		}

		//block not present in palette
		if ((int&)oldPaletteEntry->block.id == 0) blockCount++;
		else if ((int&)bl.id == 0) blockCount--;

		//test the refCount for the old block
		if (oldPaletteEntry->referenceCount == 1)
		{
			//put the new block in the old entry
			oldPaletteEntry->block.set((int&)bl.id);
			blockStates->setElement(blockStatesIndex, (int&)bl.id);
		}
		else
		{
			//create a different palette entry for the new block
			oldPaletteEntry->referenceCount--;
			palette.emplace_back(bl, 1);
			updateBitsPerBlock();
			blockStates->setElement(blockStatesIndex, (const int&)bl.id);
		}
	}
	else
	{
		PaletteEntry* oldPaletteEntry = &palette[blockStatesValue];

		if ((int&)oldPaletteEntry->block.id == (const int&)bl.id)
			//the specified coordinates already contain the specified block
			return;

		//check if the block is already in the palette
		ull i = 0;
		for (PaletteEntry& entry : palette)
		{
			if ((int&)entry.block.id == (const int&)bl.id)
			{
				//block found in the palette
				if ((int&)oldPaletteEntry->block.id == 0) blockCount++;
				else if ((int&)bl.id == 0) blockCount--;
				blockStates->setElement(blockStatesIndex, i);

				//take care of the new block
				entry.referenceCount++;

				//take care of the previous block
				//update old refCount and bitsPerBlock if necessary
				decRefCount(*oldPaletteEntry, blockStatesValue);

				return;
			}
			i++;
		}

		//block not present in palette
		if ((int&)oldPaletteEntry->block.id == 0) blockCount++;
		else if ((int&)bl.id == 0) blockCount--;

		//test the refCount for the old block
		if (oldPaletteEntry->referenceCount == 1)
		{
			//put the new block in the old entry
			oldPaletteEntry->block.set((int&)bl.id);
		}
		else
		{
			//create a different palette entry for the new block
			oldPaletteEntry->referenceCount--;
			palette.emplace_back(bl, 1);
			updateBitsPerBlock();
			if (useGlobalPallete)
			{
				blockStates->setElement(blockStatesIndex, (const int&)bl.id);
			}
			else
			{
				ull paletteSize = palette.size() - 1;
				blockStates->setElement(blockStatesIndex, paletteSize);
			}
		}
	}
}

const varInt LightSection::lightArrayLength = 2048;

LightSection::~LightSection()
{
	if (skyLight) delete skyLight;
	if (blockLight) delete blockLight;
}