#include "region.h"
#include "../world.h"
#include "../types/error.h"
#include "../server/log.h"
#include <iostream>
#include "../types/basic.h"

class chunkNotLoaded : public std::exception
{
	using std::exception::exception;
};

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
		Log::info() << "Incorrect chunk unload at [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "]" << Log::endl;
		throw runtimeWarning("Tried to unload a chunk the was not loaded");
	}
	chunk->loadCount--;
	IF_CHUNK_DEBUG(Log::info() << "Chunk [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "] unloaded (" << chunk->loadCount << ")" << Log::endl);
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
		Log::info() << "Incorrect chunk set in region [ " << rX << ", " << rX << "], relative chunk [" << relX << ", " << relZ << "]" << Log::endl;
		//throw runtimeError("Tried to set a chunk when chunk was already set");
		return;
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

BlockState& Region::getPaletteEntry(int relX, int relY, int relZ)
{
	Chunk* chunk = chunks[relX >> 4][relZ >> 4];
	if (!chunk) throw chunkNotLoaded("Region::getPaletteEntry(int3): Chunk not loaded");
	return chunk->getPaletteEntry(relX & 0xf, relY, relZ & 0xf);
}
BlockState& Region::getPaletteEntry(int chunkX, int sectionY, int chunkZ, int paletteIndex)
{
	Chunk* chunk = chunks[chunkX][chunkZ];
	if (!chunk) throw chunkNotLoaded("Region::getPaletteEntry(int4): Chunk not loaded");
	return chunk->getPaletteEntry(sectionY, paletteIndex);
}
BlockState Region::getBlock(int relX, int relY, int relZ)
{
	Chunk* chunk = chunks[relX >> 4][relZ >> 4];
	if (!chunk) throw chunkNotLoaded("Region::getBlock: Chunk not loaded");
	return chunk->getBlock(relX & 0xf, relY, relZ & 0xf);
}
void Region::setBlock(int relX, int relY, int relZ, const BlockState& bl)
{
	Chunk* chunk = chunks[relX >> 4][relZ >> 4];
	if (!chunk) throw chunkNotLoaded("Region::setBlock: Chunk not loaded");
	chunk->setBlock(relX & 0xf, relY, relZ & 0xf, bl);
}