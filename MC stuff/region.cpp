#include "region.h"
#include "world.h"

Region::Region(int rX, int rZ) : rX(rX), rZ(rZ) { }
Region::~Region()
{
	for (int z = 0; z < 32; z++) for (int x = 0; x < 32; x++) if (chunks[x][z]) delete chunks[x][z];
}

Chunk* Region::load(World* parent, int relX, int relZ)
{
	//try to read chunk from region file, if found
	return nullptr;
}
void Region::unload(World* parent)
{
	for (int z = 0; z < 32; z++) for (int x = 0; x < 32; x++) if (chunks[x][z]) unload(parent, x, z);
}
void Region::unload(World* parent, int relX, int relZ)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (!(chunk))
	{
		std::cout << "Incorrect chunk unload at [" << rX + relX << ", " << rZ + relZ << "]\n";
		throw runtimeWarning("Tries to unload a chunk the was not loaded");
	}
	chunk->loadCount--;
	if (!chunk->loadCount)
	{
		//write the chunk to file
		delete chunk;
		chunk = 0;
	}
}
void Region::set(int relX, int relZ, Chunk* p)
{
	chunks[relX][relZ] = p;
	p->loadCount = 1;
}

Chunk* Region::get(World* parent, int relX, int relZ)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (chunk)
	{
		chunk->loadCount++;
		return chunk;
	}
	//try to load it from file
	chunk = load(parent, relX, relZ);
	if (chunk)
	{
		chunk->loadCount = 1;
	}
	return chunk;
}