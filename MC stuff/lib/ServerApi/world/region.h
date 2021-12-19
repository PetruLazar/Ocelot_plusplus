#pragma once
#include "chunk.h"
#include <fstream>

class World;

class Region
{
	Chunk* chunks[32][32] = { 0 };
	int loadedChunks = 0;

	struct HeaderEntry
	{
		ush fileOffset; //in 1024B sections
		ush length; //in 1024B sections
	} regionHeader[32][32];
	//is the i-th section occupied?
	std::vector<bool> regionFileMap;
	std::fstream regionFile;

	//void writeChunkToFile(char* data, uint size);

public:
	int rX, rZ;

	SERVER_API Region(const mcString& worldName, int rX, int rZ);
	SERVER_API ~Region();

	SERVER_API void save(int relX, int relZ, bool autoFlush = true);
	SERVER_API Chunk* load(World*, int relX, int relZ);

	SERVER_API void unload(World*);
	SERVER_API void unload(World*, int relX, int relZ);
	SERVER_API void set(int relX, int relZ, Chunk*);
	SERVER_API Chunk* get(World* world, int relX, int relZ, bool increaseLoadCount = false);
	SERVER_API bool hasChunksLoaded();

	SERVER_API BlockState& getPaletteEntry(int relX, int relY, int relZ);
	SERVER_API BlockState& getPaletteEntry(int chunkX, int sectionY, int chunkZ, int paletteIndex);
	SERVER_API BlockState getBlock(int relX, int relY, int relZ);
	SERVER_API void setBlock(int relX, int relY, int relZ, const BlockState&);
};