#pragma once
#include "world/region.h"
#include "types/mcString.h"
#include "types/position.h"

typedef Chunk* (*GeneratorFunction)(World*, int, int);

class World
{
	std::vector<Region*> regions;

	static Chunk* generate_def(World*, int x, int z);
	static Chunk* generate_flat(World*, int x, int z);

public:
	World(const char* name);
	~World();

	mcString name;

	struct {
		Position Absolute;

		bdouble X, Y, Z;
		bfloat Yaw, Pitch;
		int ChunkX, ChunkZ;
	} spawn;

	/*Contents of nbt_compound characteristics
	*
	* piglin safe - byte
	* natural - byte
	* ambient_light - float (0 - 1)
	* fixed_time - optional long (0 - 24 000)
	* infiniburn - string
	* respawn_anchor_works - byte
	* has_skylight - byte
	* bed_works - byte
	* effects - string
	* has_raids - byte
	* min_y - int
	* height - int
	* logical_height - int (0 - 256)
	* coordinate_scale - float (0.00001 (1e-5) - 30000000 (3e7))
	* ultrawarm - byte
	* has_ceiling - byte
	*/
	nbt_compound characteristics;
	int height;
	int min_y;
	bool isFlat;

	SERVER_API void unload(int x, int z);
	SERVER_API Chunk* get(int x, int z, bool increaseLoadCount = false);

	GeneratorFunction generatorFunction;

	//static members
	SERVER_API static nbt_compound dimension_codec;

	SERVER_API static void loadAll();
	SERVER_API static void unloadAll();
	//initial spawn is in first world
	SERVER_API static std::vector<World*> worlds;

	//temporaty variable
	SERVER_API static int spawnWorld;
};