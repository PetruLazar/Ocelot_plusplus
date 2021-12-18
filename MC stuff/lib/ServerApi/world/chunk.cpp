#include "chunk.h"
#include "../types/error.h"
#include "../types/utils.h"

using namespace std;

Chunk::Chunk() {}
Chunk::Chunk(ull worldHeight)
{
	//heightmap initialization
	heightmaps = new BitArray(256, bitCount(worldHeight));

	ull sectionCount = worldHeight >> 4;

	//section initialization
	sections.resize(sectionCount);

	//light section initialization
	lightData.resize((ull)sectionCount + 2);
	skyLightMask = new BitArray(sectionCount + 2, 1);
	blockLightMask = new BitArray(sectionCount + 2, 1);
	emptySkyLightMask = new BitArray(sectionCount + 2, 1);
	emptyBlockLightMask = new BitArray(sectionCount + 2, 1);

	//section mask initialization
	sectionMask = new BitArray(sectionCount, 1);
}
Chunk::~Chunk()
{
	//chunk data
	delete heightmaps;
	delete sectionMask;

	//light data
	delete skyLightMask;
	delete blockLightMask;
	delete emptySkyLightMask;
	delete emptyBlockLightMask;
}

//reading a chunk that already contains data causes a memory leak!
void Chunk::read(std::istream& file)
{
	//chunk data

	//read section mask
	sectionMask->read(file);

	//read heightmap
	heightmaps->read(file);

	for (Section& sec : sections)
	{
		//read biomes
		file.read((char*)(&sec.biomes), 256);

		//read useGlobalPalette, bitsPerBlock and blockCount
		file.read((char*)(&sec.useGlobalPallete), 4);

		//read paletteSize
		ush paletteSize;
		file.read((char*)(&paletteSize), 2);

		//read palette
		sec.palette.clear();
		sec.palette.reserve(paletteSize);

		for (ush i = 0; i < paletteSize; i++)
		{
			//read each palette entry
			int stateid;
			short refCount;
			file.read((char*)(&stateid), 4);
			file.read((char*)(&refCount), 2);

			sec.palette.emplace_back(stateid, refCount);
		}

		//read blockStates
		sec.blockStates = new BitArray(4096, sec.bitsPerBlock);
		sec.blockStates->read(file);

	}

	//light data

	//read light data

	//read light masks
	skyLightMask->read(file);
	blockLightMask->read(file);
	emptySkyLightMask->read(file);
	emptyBlockLightMask->read(file);

	//read light data for each section
	for (uint i = 0; i < lightData.size(); i++)
	{
		LightSection& sec = lightData[i];
		sec.skyLight = new BitArray(4096, 4);
		if (skyLightMask->getElement(i))
		{
			//bit set in skyLightMask
			sec.skyLight->read(file);
		}
		else
		{
			//bit not set in skyLightMask
			if (emptySkyLightMask->getElement(i))
			{
				//bit set in emptySkyLightMask
				//section light has all zeros
			}
			else
			{
				//bit not set in emptySkyLightMask
				//should calculate light
			}
		}

		sec.blockLight = new BitArray(4096, 4);
		if (blockLightMask->getElement(i))
		{
			//bit set in blockLightMask
			sec.blockLight->read(file);
		}
		else
		{
			//bit not set in skyLightMask
			if (emptyBlockLightMask->getElement(i))
			{
				//bit set in emptyBlockLightMask
				//section light has all zeros
			}
			else
			{
				//bit not set in emptyBlockLightMask
				//should calculate light
			}
		}
	}

	//throw runtimeError("Chunk::read not implemented yet.");
}
void Chunk::write(ostream& file)
{
	//chunk data

	//write sectionMask and heightmap
	sectionMask->write(file);
	heightmaps->write(file);

	//write each section
	for (Section& sec : sections)
	{
		//write biomes
		file.write((char*)(&sec.biomes), 256);

		//write useGlobalPalette, bitsPerBlock, blockCount
		file.write((char*)(&sec.useGlobalPallete), 4);

		//write paletteSize
		ush paletteSize = sec.palette.size();
		file.write((char*)(&paletteSize), 2);

		//write palette
		for (PaletteEntry& entry : sec.palette)
		{
			//read each palette entry
			file.write((char*)(&entry.block.id), 4);
			file.write((char*)(&entry.referenceCount), 2);
		}

		//write blockStates
		sec.blockStates->write(file);
	}

	//light data

	//write light masks
	skyLightMask->write(file);
	blockLightMask->write(file);
	emptySkyLightMask->write(file);
	emptyBlockLightMask->write(file);

	//write light data for each section
	for (uint i = 0; i < lightData.size(); i++)
	{
		LightSection& sec = lightData[i];

		if (skyLightMask->getElement(i))
		{
			sec.skyLight->write(file);
		}

		if (blockLightMask->getElement(i))
		{
			sec.blockLight->write(file);
		}
	}

	//throw runtimeError("Chunk::write not implemented yet.");
}

BlockState& Chunk::getPaletteEntry(int relX, int relY, int relZ)
{
	return sections[relY >> 4].getPaletteEntry(relX, relY & 0xf, relZ);
}
BlockState& Chunk::getPaletteEntry(int sectionY, int paletteIndex)
{
	return sections[sectionY].getPaletteEntry(paletteIndex);
}
BlockState Chunk::getBlock(int relX, int relY, int relZ)
{
	return sections[relY >> 4].getBlock(relX, relY & 0xf, relZ);
}
void Chunk::setBlock(int relX, int relY, int relZ, const BlockState& bl)
{
	Section& section = sections[relY >> 4];
	bool hadBlocks = section.blockCount;
	section.setBlock(relX, relY & 0xf, relZ, bl);
	if ((bool)section.blockCount != hadBlocks)
	{
		//section mask modified
		sectionMask->setElement(relY >> 4, (bool)section.blockCount);
	}

	//change heightmaps? (once it is needed)

	//updatelight
}

void Chunk::writeSectionData(char*&)
{
	throw runtimeError("Chunk::writeSectionData not implemented yet");
}