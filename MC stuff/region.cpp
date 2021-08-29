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
	loadedChunks = 0;
}
void Region::unload(World* parent, int relX, int relZ)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (!(chunk))
	{
		std::cout << "\nIncorrect chunk unload at [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "]";
		throw runtimeWarning("Tried to unload a chunk the was not loaded");
	}
	chunk->loadCount--;
	IF_CHUNK_DEBUG(std::cout << "\nChunk [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "] unloaded (" << chunk->loadCount << ")");
	if (!chunk->loadCount)
	{
		//write the chunk to file

		delete chunk;
		chunk = 0;
		loadedChunks--;
	}
}
void Region::set(int relX, int relZ, Chunk* p)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (chunk)
	{
		std::cout << "\nIncorrect chunk set in region [ " << rX << ", " << rX << "], relative chunk [" << relX << ", " << relZ << "]";
		throw runtimeError("Tried to set a chunk when chunk was already set");
	}
	chunks[relX][relZ] = p;
	p->loadCount = 1;
	loadedChunks++;
}

Chunk* Region::get(World* parent, int relX, int relZ, bool increaseLoadCount)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (chunk)
	{
		if (increaseLoadCount) chunk->loadCount++;
		return chunk;
	}
	//try to load it from file
	chunk = load(parent, relX, relZ);
	if (chunk)
	{
		chunk->loadCount = 1;
		loadedChunks++;
	}
	return chunk;
}

bool Region::hasChunksLoaded() { return loadedChunks; }