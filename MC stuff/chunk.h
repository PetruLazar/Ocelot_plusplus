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
	//Chunk();
	~Chunk();

	static const int max_inflated_chunkSize = 64 * 1024;
	static const int max_deflated_chunkSize = 128 * 1024;

	ull loadCount; // 

	//varInt sectionMaskLength;
	//blong* sectionMask;

	BitArray* heightmaps;

	//biomes are stored in sections

	//data
	std::vector<Section> sections;

	varInt nOfBlockEntities;
	//block Entities

	//for loading/saving from/to file
	void read(std::fstream&, int sectionCount);
	void write(std::fstream&);
};

