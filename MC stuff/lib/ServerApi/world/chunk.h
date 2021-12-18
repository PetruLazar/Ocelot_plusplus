#pragma once
#include "../nbt.h"
#include "section.h"

class Chunk
{
public:
	SERVER_API Chunk();
	SERVER_API Chunk(ull heightInBlocks);
	SERVER_API ~Chunk();

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
	SERVER_API void read(std::istream& file);
	SERVER_API void write(std::ostream& file);
	//writing and reading from buffer after/before compression

	//get direct access to the palette, allowing you to modify all the blocks with the same state in the section
	SERVER_API BlockState& getPaletteEntry(int relX, int relY, int relZ);
	//get direct access to the palette, allowing you to modify all the blocks with the same state in the section
	SERVER_API BlockState& getPaletteEntry(int sectionY, int paletteIndex);
	//get a copy of the block state at the desired coordinates to use with setBlock
	SERVER_API BlockState getBlock(int relX, int relY, int relZ);
	SERVER_API void setBlock(int relX, int relY, int relZ, const BlockState&);

	//includes data size field of the packet
	SERVER_API void writeSectionData(char*&);
};

