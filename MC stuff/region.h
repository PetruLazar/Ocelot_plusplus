#pragma once
#include "basic.h"
#include "chunk.h"

class World;

class Region
{
	Chunk* chunks[32][32] = { 0 };
	int loadedChunks = 0;

public:
	int rX, rZ;

	Region(int rX, int rZ);
	~Region();

	Chunk* load(World*, int relX, int relZ);
	void unload(World*);
	void unload(World*, int relX, int relZ);
	void set(int relX, int relZ, Chunk*);
	Chunk* get(World* world, int relX, int relZ,bool increaseLoadCount = false);
	bool hasChunksLoaded();
};

