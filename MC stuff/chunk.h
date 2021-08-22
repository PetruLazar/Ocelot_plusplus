#pragma once
#include "nbt.h"
#include "section.h"

class Chunk
{
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
	void read(char*&, int sectionCount, int sectionOffset = 0);
	void write(char*&);
	//writing and reading from buffer after/before compression

	//includes data size field of the packet
	void writeSectionData(char*&);
};

