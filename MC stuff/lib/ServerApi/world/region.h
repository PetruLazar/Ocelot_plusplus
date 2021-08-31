#pragma once
#include "chunk.h"

class World;

class Region
{
	Chunk* chunks[32][32] = { 0 };
	int loadedChunks = 0;

public:
	int rX, rZ;

	SERVER_API Region(int rX, int rZ);
	SERVER_API ~Region();

	SERVER_API Chunk* load(World*, int relX, int relZ);
	SERVER_API void unload(World*);
	SERVER_API void unload(World*, int relX, int relZ);
	SERVER_API void set(int relX, int relZ, Chunk*);
	SERVER_API Chunk* get(World* world, int relX, int relZ,bool increaseLoadCount = false);
	SERVER_API bool hasChunksLoaded();
};

