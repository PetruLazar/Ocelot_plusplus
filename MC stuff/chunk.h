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

	//light data

	//masks & empty masks
	BitArray* skyLightMask;
	BitArray* blockLightMask;
	BitArray* emptySkyLightMask;
	BitArray* emptyBlockLightMask;

	//arrays
	std::vector<LightSection> lightData;

	//chunk data
	ull loadCount; // 

	BitArray* sectionMask;

	BitArray* heightmaps;

	//biomes are stored in sections

	//data
	std::vector<Section> sections;

	varInt nOfBlockEntities;
	//block Entities

	//for loading/saving from/to file
	void read(std::fstream&, int sectionCount);
	void write(std::fstream&);

	//includes data size field of the packet
	void writeSectionData(char*&);
};

