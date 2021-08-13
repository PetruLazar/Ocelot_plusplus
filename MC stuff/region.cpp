#include "region.h"

Region::Region(int rX, int rZ) : rX(rX), rZ(rZ) { }

void Region::load(int relX, int relZ)
{
	throw "WIP";
}
void Region::save(int relX, int relZ)
{
	throw "WIP";
}
void Region::set(int relX, int relZ, Chunk* p)
{
	chunks[relX][relZ] = p;
}

Chunk* Region::get(int relX, int relZ)
{
	return chunks[relX][relZ];
}