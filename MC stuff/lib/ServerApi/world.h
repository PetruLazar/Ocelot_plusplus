#pragma once
#include "world/region.h"
#include "types/mcString.h"
#include "types/position.h"
#include "types/entity.h"
#include <Windows.h>
#include "types/slot.h"
#include "types/enums.h"
#include <mutex>
#include <forward_list>

class Player;
#include "player/player.h"

typedef Chunk* (*GeneratorFunction)(World*, int, int);

class World
{
	std::vector<Region*> regions;

	static Chunk* generate_def(World*, int x, int z);
	static Chunk* generate_flat(World*, int x, int z);
	static Chunk* generate_void(World*, int x, int z);

	std::vector<Entity::entity*> entities;

	eidDispenser::Entity eidDispenser;

public:
	SERVER_API World(const char* name);
	SERVER_API ~World();

	mcString name;
	std::forward_list<Player*> players;

	eidDispenser::Entity* getEidDispenser();
	void addEntity(Entity::entity* entity);
	bool removeEntity(varInt eid);
	std::vector<Entity::entity*> getEntitiesByType(Entity::type theType);

	/*Contents of nbt_compound characteristics
	*
	* piglin safe - Byte
	* natural - Byte
	* ambient_light - float (0 - 1)
	* fixed_time - optional long (0 - 24 000)
	* infiniburn - #string
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
	struct
	{
		Position Absolute;

		bdouble X, Y, Z;
		bfloat Yaw, Pitch;
		int ChunkX, ChunkZ;
	} spawn;
	SERVER_API void setSpawn(bdouble X, bdouble Y, bdouble Z);
	SERVER_API void setSpawn(bdouble X, bdouble Y, bdouble Z, bfloat Yaw, bfloat Pitch);
	SERVER_API void loadArea(int chunkX, int chunkZ, int radius);
	SERVER_API void unloadArea(int chunkX, int chunkZ, int radius);
	SERVER_API void updateMainFile();

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
	SERVER_API Chunk* getChunk(int x, int z, bool increaseLoadCount = false);
	SERVER_API Region* getRegion(int rX, int rZ);

	//converts a y coordinate from absolute values to relative values (to the min_y of the world)
	SERVER_API int AbsToRelHeight(int);
	SERVER_API int RelToAbsHeight(int);
	//checks whether the y coordinate is in world (relative coordinate)
	SERVER_API bool checkCoordinates(int y);
	SERVER_API int getBlock(int x, int y, int z);
	SERVER_API void setBlock(int x, int y, int z, int blockid, nbt_compound* nbt_data = nullptr, Player* broadcastException = nullptr);
	SERVER_API void setBlockNoBroadcast(int x, int y, int z, int blockid, nbt_compound* nbt_data = nullptr);
	SERVER_API void setBlockByItem(Player*, Slot*, Position, BlockFace, bfloat curX, bfloat curY, bfloat curZ);

	//generator
	GeneratorFunction generatorFunction = 0;
	HMODULE generatorModule = 0;

	//time
	int randomTickSpeed = 3;
	SERVER_API void tick();

	//static members
	SERVER_API static nbt_compound dimension_codec;
	SERVER_API static const Byte currentBiomeBitsPerEntry;
	static constexpr int currentWorldVersion = 0;

	SERVER_API static bool loadAll();
	SERVER_API static void unloadAll();

	SERVER_API static std::forward_list<World*> worlds;
	SERVER_API static World* getWorld(const mcString& worldName);

	//temporary variable
	SERVER_API static World* spawnWorld;
};