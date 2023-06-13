#pragma once

#include <forward_list>

#include "../nbt.h"
#include "section.h"
#include "../types/entity.h"

class Player;
//#include "../player/player.h"

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
	std::vector<LightSection> lightSections;

	//chunk data
	ull loadCount = 0;

	BitArray* sectionMask;

	BitArray* heightmaps;

	//biomes are stored in sections

	//data
	std::vector<Section> sections;

	//block Entities
	//varInt nOfBlockEntities;
	std::vector<BlockEntity*> blockEntities;

	//currently unused
	std::forward_list<Player*> players;
	std::forward_list<Entity::entity*> entities;

	//for loading/saving from/to file
	SERVER_API void read(std::istream& file);
	SERVER_API void write(std::ostream& file);
	//writing and reading from buffer after/before compression

	//get a copy of the block state at the desired coordinates to use with setBlock
	int getBlock(int relX, int relY, int relZ) { return sections[relY >> 4].getBlock(relX, relY & 0xf, relZ); }
	SERVER_API nbt_compound* getNbt(int relX, int relY, int relZ);
	int getBlockEntityIndex(int relX, int relY, int relZ);
	SERVER_API bool setBlock(int relX, int relY, int relZ, int blockid, nbt_compound* nbt_data = nullptr);
	Byte getSkyLight(int relX, int relY, int relZ) { return lightSections[((ull)relY >> 4) + 1].getSkyLight(relX, relY & 0xf, relZ); }
	SERVER_API void setSkyLight(int relX, int relY, int relZ, Byte value);
	Byte getBlockLight(int relX, int relY, int relZ) { return lightSections[((ull)relY >> 4) + 1].getBlockLight(relX, relY & 0xf, relZ); }
	SERVER_API void setBlockLight(int relX, int relY, int relZ, Byte value);

	//includes data size field of the packet
	SERVER_API void writeSectionData(char*&);

	void addPlayer(Player* p) { players.emplace_front(p); }
	void removePlayer(Player* p) { players.remove(p); }
	void addEntity(Entity::entity* en) { entities.emplace_front(en); }
	void removeEntity(Entity::entity* en) { entities.remove(en); }

	SERVER_API void tick(World* wld, int cX, int cZ, int randomTickSpeed);
};

