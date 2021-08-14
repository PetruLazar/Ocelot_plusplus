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

	static const int max_inflated_chunkSize = 64 * 1024;
	static const int max_deflated_chunkSize = 128 * 1024;

	ull playerCount; // 

	//varInt sectionMaskLength;
	//blong* sectionMask;

	short motion_blocking[16][16];

	//biomes are stored in sections

	//data
	std::vector<Section> sections;

	varInt nOfBlockEntities;
	//block Entities

	//for loading/saving from/to file
	void read(std::fstream&);
	void write(std::fstream&);
};

