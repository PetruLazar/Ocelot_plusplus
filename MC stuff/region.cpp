#include "region.h"

Region::Region(int rX, int rZ) : rX(rX), rZ(rZ) { }
Region::~Region()
{
	for (int z = 0; z < 32; z++) for (int x = 0; x < 32; x++) if (chunks[x][z]) delete chunks[x][z];
}

Chunk* Region::load(const mcString&, int relX, int relZ)
{
	throw runtimeError("Region::load not implemented yet.");
}
void Region::unload(const mcString& worldName)
{
	for (int z = 0; z < 32; z++) for (int x = 0; x < 32; x++) if (chunks[x][z]);// unload(worldName, x, z);
}
void Region::unload(const mcString& worldName, int relX, int relZ)
{
	throw runtimeError("Region::unload not implemented yet.");
}
void Region::set(int relX, int relZ, Chunk* p)
{
	chunks[relX][relZ] = p;
}

Chunk* Region::get(const mcString& worldName, int relX, int relZ)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (chunk)
	{
		return chunk;
	}
	//try to load it from file
	//load(worldName, relX, relZ);
	return chunk;
}