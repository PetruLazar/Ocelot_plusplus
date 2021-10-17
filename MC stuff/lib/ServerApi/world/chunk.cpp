#include "chunk.h"
#include "../types/error.h"
#include "../types/utils.h"

using namespace std;

Chunk::Chunk() { }
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

void Chunk::read(char*& buffer, int sectionCount, int sectionOffset)
{
	//chunk data


	//light data


	throw runtimeError("Chunk::read not implemented yet.");
}
void Chunk::write(char*& buffer)
{
	//chunk data


	//light data


	throw runtimeError("Chunk::write not implemented yet.");
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
}

void Chunk::writeSectionData(char*&)
{
	throw runtimeError("Chunk::writeSectionData not implemented yet");
}