#include "chunk.h"
#include "basic.h"

using namespace std;

Chunk::~Chunk()
{
	delete heightmaps;
}

void Chunk::read(std::fstream& f, int sectionCount)
{
	throw runtimeError("Chunk::read not implemented yet.");
}
void Chunk::write(std::fstream& f)
{
	throw runtimeError("Chunk::write not implemented yet.");
}
void Chunk::writeSectionData(char*&)
{
	throw runtimeError("Chunk::writeSectionData not implemented yet");
}