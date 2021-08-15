#pragma once
#include "basic.h"
#include "chunk.h"

class Region
{
	Chunk* chunks[32][32] = { 0 };

public:
	int rX, rZ;

	Region(int rX, int rZ);
	~Region();

	Chunk* load(const mcString&, int relX, int relZ);
	void unload(const mcString&);
	void unload(const mcString&, int relX, int relZ);
	void set(int relX, int relZ, Chunk*);
	Chunk* get(const mcString& worldName, int relX, int relZ);
};

