#include "chunk.h"
#include "../types/error.h"

using namespace std;

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