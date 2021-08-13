#pragma once
#include "basic.h"
#include "chunk.h"

class Region
{
	Chunk* chunks[32][32];

public:
	int rX, rZ;

	Region(int rX, int rZ);

	void load(int relX, int relZ);
	void save(int relX, int relZ);
	void set(int relX, int relZ, Chunk*);
	Chunk* get(int relX, int relZ);
};

