#pragma once
#include "nbt.h"
#include "section.h"

class Chunk
{
	enum class compressionMethod : byte
	{
		gzipCompression = 1,
		zlibCompression,
		none
	};

public:
	//not actually empty, has a platform of stone
	//static nbt_compound emptyChunk;

	Chunk();

	static const int max_inflated_chunkSize = 64 * 1024;
	static const int max_deflated_chunkSize = 128 * 1024;

	ull playerCount; // 

	varInt sectionMaskLength;
	blong* sectionMask;

	nbt_compound heightmaps;

	//biomes are stored in sections

	//data
	std::vector<Section> sections;

	varInt nOfBlockEntities;
	//block Entities

	//for loading/saving from/to file
	void read(std::fstream&);
	void write(std::fstream&);
};

