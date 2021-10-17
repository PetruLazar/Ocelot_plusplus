#pragma once
#include "world/region.h"
#include "types/mcString.h"
#include "types/position.h"
#include "types/entity.h"
#include <Windows.h>

class Player;
#include "player/player.h"

typedef Chunk* (*GeneratorFunction)(World*, int, int);

class World
{
	std::vector<Region*> regions;

	static Chunk* generate_def(World*, int x, int z);
	static Chunk* generate_flat(World*, int x, int z);
	static Chunk* generate_void(World*, int x, int z);

public:
	World(const char* name);
	~World();

	mcString name;
	std::vector<Player*> players;

	/*Contents of nbt_compound characteristics
	*
	* piglin safe - Byte
	* natural - Byte
	* ambient_light - float (0 - 1)
	* fixed_time - optional long (0 - 24 000)
	* infiniburn - string
	* respawn_anchor_works - Byte
	* has_skylight - Byte
	* bed_works - Byte
	* effects - string
	* has_raids - Byte
	* min_y - int
	* height - int
	* logical_height - int (0 - 256)
	* coordinate_scale - float (0.00001 (1e-5) - 30000000 (3e7))
	* ultrawarm - Byte
	* has_ceiling - Byte
	*/
	nbt_compound characteristics;
	struct {
		Position Absolute;

		bdouble X, Y, Z;
		bfloat Yaw, Pitch;
		int ChunkX, ChunkZ;
	} spawn;
	bool isFlat;
	enum GeneratorType : Byte
	{
		customWorld = 'c',
		voidWorld = 'v',
		flatWorld = 'f',
		defaultWorld = 'd'
	} generatorType;

	int height;
	int min_y;

	SERVER_API void unload(int x, int z);
	SERVER_API Chunk* get(int x, int z, bool increaseLoadCount = false);

	//generator
	GeneratorFunction generatorFunction = 0;
	HMODULE generatorModule = 0;

	//static members
	SERVER_API static nbt_compound dimension_codec;

	SERVER_API static eidDispenser::Entity eidDispenser;

	SERVER_API static void loadAll();
	SERVER_API static void unloadAll();

	SERVER_API static std::vector<World*> worlds;

	//temporary variable
	SERVER_API static int spawnWorld;
};