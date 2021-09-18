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
	throw runtimeError("Chunk::read not implemented yet.");
}
void Chunk::write(char*& buffer)
{
	//chunk data


	//light data


	throw runtimeError("Chunk::write not implemented yet.");
}
void Chunk::writeSectionData(char*&)
{
	throw runtimeError("Chunk::writeSectionData not implemented yet");
}