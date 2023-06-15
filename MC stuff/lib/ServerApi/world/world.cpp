#include "../world.h"
#include "../types/utils.h"
#include "../server/server.h"

#include "noise.h"
#include "../debug/mcexceptions.h"
#include "../types/basic.h"
#include "../debug/log.h"
#include "../server/options.h"
#include "../player/message.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

const int terrainHeightAverage = 85;
const int terrainHeightAmplitude = 35;
const double noiseFactor_x = 1. / 128, noiseFactor_z = 1. / 128;

std::forward_list<World*> World::worlds;
World* World::spawnWorld;

nbt_compound World::dimension_codec("", new nbt*[2]{
	new nbt_compound("minecraft:dimension_type",new nbt * [2]{
		new nbt_string("type","minecraft:dimension_type"),
		new nbt_list("value",new nbt * [2]{
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:overworld"),
				new nbt_int("id",0),
				new nbt_compound("element",new nbt * [15]{
					new nbt_Byte("piglin_safe",0),
					new nbt_Byte("natural",1),
					new nbt_float("ambient_light",0.f),
					new nbt_string("infiniburn","#minecraft:infiniburn_overworld"),
					new nbt_Byte("respawn_anchor_works",0),
					new nbt_Byte("has_skylight",1),
					new nbt_Byte("bed_works",1),
					new nbt_string("effects","minecraft:overworld"),
					new nbt_Byte("has_raids",1),
					new nbt_int("min_y",0),
					new nbt_int("height",256),
					new nbt_int("logical_height",256),
					new nbt_float("coordinate_scale",1.f),
					new nbt_Byte("ultrawarm",0),
					new nbt_Byte("has_ceiling",0)
				},15)
			},3),
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:fakeworld"),
				new nbt_int("id",1),
				new nbt_compound("element",new nbt * [15]{
					new nbt_Byte("piglin_safe",1),
					new nbt_Byte("natural",0),
					new nbt_float("ambient_light",1.f),
					new nbt_string("infiniburn","#"),
					new nbt_Byte("respawn_anchor_works",0),
					new nbt_Byte("has_skylight",1),
					new nbt_Byte("bed_works",0),
					new nbt_string("effects","minecraft:the_end"),
					new nbt_Byte("has_raids",0),
					new nbt_int("min_y",0),
					new nbt_int("height",256),
					new nbt_int("logical_height",256),
					new nbt_float("coordinate_scale",1.f),
					new nbt_Byte("ultrawarm",0),
					new nbt_Byte("has_ceiling",0)
				},15)
			},3)

		},2)
	}, 2),
	new nbt_compound("minecraft:worldgen/biome",new nbt * [2]{
		new nbt_string("type","minecraft:worldgen/biome"),
		new nbt_list("value",new nbt * [10]{
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:plains"),
				new nbt_int("id",0),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","rain"),
					new nbt_float("depth", .125f),
					new nbt_float("temperature", .8f),
					new nbt_float("scale", .05f),
					new nbt_float("downfall", .4f),
					new nbt_string("category", "plains"),
					new nbt_compound("effects",new nbt * [5] {
						new nbt_int("sky_color",7907327),
						new nbt_int("water_fog_color",329011),
						new nbt_int("fog_color",12638463),
						new nbt_int("water_color",4159204),
						new nbt_compound("mood_sound", new nbt * [4]{
							new nbt_int("tick_delay", 6000),
							new nbt_double("offset", 2.),
							new nbt_string("sound", "minecraft:ambient.cave"),
							new nbt_int("block_search_extend", 8)
						},4)
					},5)
				},7)
			},3),
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","uab:domsa"),
				new nbt_int("id",1),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0xff),
						new nbt_int("water_fog_color",0xff),
						new nbt_int("fog_color",0xff),
						new nbt_int("water_color",0xff),
						new nbt_int("foliage_color",0xff),
						new nbt_int("grass_color",0xff)
					},6)
				},7)
			},3),
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:taiga"),
				new nbt_int("id",2),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0xff00),
						new nbt_int("water_fog_color",0xff00),
						new nbt_int("fog_color",0xff00),
						new nbt_int("water_color",0xff00),
						new nbt_int("foliage_color",0xff00),
						new nbt_int("grass_color",0xff00)
					},6)
				},7)
			},3),
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:ocean"),
				new nbt_int("id",3),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0xff0000),
						new nbt_int("water_fog_color",0xff0000),
						new nbt_int("fog_color",0xff0000),
						new nbt_int("water_color",0xff0000),
						new nbt_int("foliage_color",0xff0000),
						new nbt_int("grass_color",0xff0000)
					},6)
				},7)
			},3),
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:mountains"),
				new nbt_int("id",4),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0xffff00),
						new nbt_int("water_fog_color",0xffff00),
						new nbt_int("fog_color",0xffff00),
						new nbt_int("water_color",0xffff00),
						new nbt_int("foliage_color",0xff0000),
						new nbt_int("grass_color",0xffff00)
					},6)
				},7)
			},3),
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:beach"),
				new nbt_int("id",5),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0xff00ff),
						new nbt_int("water_fog_color",0xff00ff),
						new nbt_int("fog_color",0xff00ff),
						new nbt_int("water_color",0xff00ff),
						new nbt_int("foliage_color",0xff00ff),
						new nbt_int("grass_color",0xff00ff)
					},6)
				},7)
			},3),
			new nbt_compound("", new nbt * [3]{
				new nbt_string("name","minecraft:desert"),
				new nbt_int("id",6),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0x00ffff),
						new nbt_int("water_fog_color",0x00ffff),
						new nbt_int("fog_color",0x00ffff),
						new nbt_int("water_color",0x00ffff),
						new nbt_int("foliage_color",0x00ffff),
						new nbt_int("grass_color",0x00ffff)
					},6)
				},7)
			}, 3),
			new nbt_compound("", new nbt * [3]{
				new nbt_string("name","minecraft:void"),
				new nbt_int("id",7),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0xffffff),
						new nbt_int("water_fog_color",0xffffff),
						new nbt_int("fog_color",0xffffff),
						new nbt_int("water_color",0xffffff),
						new nbt_int("foliage_color",0xffffff),
						new nbt_int("grass_color",0xffffff)
					},6)
				},7)
			}, 3),
			new nbt_compound("", new nbt * [3]{
				new nbt_string("name","lazorenii:custom"),
				new nbt_int("id",8),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0x51fda3),
						new nbt_int("water_fog_color",0xa2619c),
						new nbt_int("fog_color",0x74b96a),
						new nbt_int("water_color",0xa15948),
						new nbt_int("foliage_color",0xf501c5),
						new nbt_int("grass_color",0x445f2c)
					},6)
				},7)
			}, 3),
			new nbt_compound("", new nbt * [3]{
				new nbt_string("name","suck:a_dick"),
				new nbt_int("id",9),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0x3adf15),
						new nbt_int("water_fog_color",0xc9162a),
						new nbt_int("fog_color",0xa69b47),
						new nbt_int("water_color",0x84951a),
						new nbt_int("foliage_color",0x5c105f),
						new nbt_int("grass_color",0xc2f544)
					},6)
				},7)
			}, 3)
		},10)
	},2)
	}, 2);
const Byte World::currentBiomeBitsPerEntry = 4; // 10 biomes in the global palette

World::World(const char* c_name) : name(c_name), characteristics("", nullptr)
{
	Log::debug(WORLD_LOAD_DEBUG) << "Loading world \"" << c_name << "\"..." << Log::flush;
	fstream worldMain("worlds\\" + name + "\\characteristics.bin", ios::binary | ios::in);

	if (!worldMain.is_open())
		throw runtimeError("Error: cannot open charactestics.bin for world \"" + name + "\"");
	varInt version;
	version.read(worldMain);
	if (version != currentWorldVersion)
		throw runtimeError("Error: characteristics.bin has an incorrect version for world \"" + name + "\"");
	if (!nbt::checkTag(worldMain))
		throw runtimeError("Error: charactestics.bin has an invalid format for world \"" + name + "\"");

	characteristics.read(worldMain);
	height = characteristics["height"].vInt();
	min_y = characteristics["min_y"].vInt();

	spawn.X.read(worldMain);
	spawn.Y.read(worldMain);
	spawn.Z.read(worldMain);
	spawn.Yaw.read(worldMain);
	spawn.Pitch.read(worldMain);

	spawn.ChunkX = int(fastfloor(spawn.X)) >> 4;
	spawn.ChunkZ = int(fastfloor(spawn.Z)) >> 4;
	spawn.Absolute = sf::Vector3i(int(fastfloor(spawn.X)), int(fastfloor(spawn.Y)), int(fastfloor(spawn.Z)));

	worldMain.read((char*)&isFlat, 1);
	worldMain.read((char*)&generatorType, 1);

	//generator selection
	switch (generatorType)
	{
	case voidWorld:
		generatorFunction = generate_void;
		break;
	case flatWorld:
		generatorFunction = generate_flat;
		break;
	case defaultWorld:
		generatorFunction = generate_def;
		break;
	case customWorld:
		try
		{
			//loading generator module
			generatorModule = LoadLibraryA(("worlds\\" + name + "\\generator.dll").c_str());
			if (!generatorModule) throw 0;
			//get address of proc for custom generator
			generatorFunction = (GeneratorFunction)GetProcAddress(generatorModule, "generate");
			if (!generatorFunction)
			{
				FreeLibrary(generatorModule);
				generatorModule = 0;
				throw 0;
			}
		}
		catch (int)
		{
			Log::info() << "Could not load custom generator for world \"" << c_name << "\", using default instead." << Log::flush;
			generatorFunction = generate_def;
		}
		break;
	}

	//if regions folder is absent, create it
	//fs::path regionsFolder();
	if (!fs::is_directory("worlds\\" + name + "\\regions"))
	{
		fs::create_directory("worlds\\" + name + "\\regions");
		//Log::info() << "\nCreated \"regions\" directory for world " << name << Log::flush;
	}

	Log::debug(WORLD_LOAD_DEBUG) << "Loading spawn area..." << Log::flush;
	loadArea(spawn.ChunkX, spawn.ChunkZ, Options::viewDistance());

	if (reinterpret_cast<long long&>(spawn.Y) == -1)
		spawn.Y = double(characteristics["min_y"].vInt()) + getChunk(spawn.ChunkX, spawn.ChunkZ)->heightmaps->getElement(((ull)spawn.Absolute.z() - ((ull)spawn.ChunkZ << 4)) * 16 + ((ull)spawn.Absolute.x() - ((ull)spawn.ChunkX << 4)));
	Log::debug(WORLD_LOAD_DEBUG) << "Done!" << Log::flush;
}

World::~World()
{
	//unload regions (and chunks, implicitly)
	for (Region* r : regions)
	{
		r->unload(this);
		delete r;
	}
	//unload entities
	for (Entity::entity* e : entities)
		delete e;
	//unload custom generator module if present
	if (generatorModule) FreeLibrary(generatorModule);

	//update characteristics.bin
}

void World::setSpawn(bdouble X, bdouble Y, bdouble Z)
{
	spawn.X = X;
	spawn.Y = Y;
	spawn.Z = Z;

	//unload the old spawn area and load the new spawn area
	int oldSpawnChunkX = spawn.ChunkX,
		oldSpawnChunkZ = spawn.ChunkZ;

	spawn.ChunkX = int(fastfloor(spawn.X)) >> 4;
	spawn.ChunkZ = int(fastfloor(spawn.Z)) >> 4;
	spawn.Absolute = sf::Vector3i(int(fastfloor(spawn.X)), int(fastfloor(spawn.Y)), int(fastfloor(spawn.Z)));

	loadArea(spawn.ChunkX, spawn.ChunkZ, Options::viewDistance());
	unloadArea(oldSpawnChunkX, oldSpawnChunkZ, Options::viewDistance());
	updateMainFile();
}
void World::setSpawn(bdouble X, bdouble Y, bdouble Z, bfloat Yaw, bfloat Pitch)
{
	spawn.Yaw = Yaw;
	spawn.Pitch = Pitch;
	setSpawn(X, Y, Z);
}
void World::loadArea(int chunkX, int chunkZ, int radius)
{
	for (int x = chunkX - radius; x <= chunkX + radius; x++)
		for (int z = chunkZ - radius; z <= chunkZ + radius; z++)
			getChunk(x, z, true);
}
void World::unloadArea(int chunkX, int chunkZ, int radius)
{
	for (int x = chunkX - radius; x <= chunkX + radius; x++)
		for (int z = chunkZ - radius; z <= chunkZ + radius; z++)
			unload(x, z);
}
void World::updateMainFile()
{
	fstream worldMain("worlds\\" + name + "\\characteristics.bin", ios::binary | ios::out);
	if (!worldMain.is_open())
		throw runtimeError("Error: cannot open charactestics.bin");

	varInt(currentWorldVersion).write(worldMain);
	characteristics.write(worldMain);

	spawn.X.write(worldMain);
	spawn.Y.write(worldMain);
	spawn.Z.write(worldMain);
	spawn.Yaw.write(worldMain);
	spawn.Pitch.write(worldMain);
	worldMain.write((char*)&isFlat, 1);
	worldMain.write((char*)&generatorType, 1);

	worldMain.close();
}

eidDispenser::Entity* World::getEidDispenser()
{
	return &this->eidDispenser;
}
void World::addEntity(Entity::entity* entity)
{
	entities.emplace_back(entity);
}
bool World::removeEntity(varInt eid)
{
	unsigned removingEntity = -1;

	for (unsigned i = 0; i < this->entities.size(); i++)
	{
		if (entities[i]->getEid() == eid)
		{
			removingEntity = i;
			break;
		}
	}

	if (removingEntity != -1)
	{
		delete entities[removingEntity];
		entities.erase(entities.begin() + removingEntity);

		return true;
	}

	return false;
}
std::vector<Entity::entity*> World::getEntitiesByType(Entity::type theType)
{
	std::vector<Entity::entity*> typeEntities;

	for (Entity::entity* entity : this->entities)
	{
		if (entity->getType() == theType)
			typeEntities.emplace_back(entity);
	}

	return typeEntities;
}

Chunk* World::generate_def(World* world, int X, int Z)
{
	int height = world->height;
	Chunk* chunk = new Chunk(height);

	uint sectionCount = world->height / 16;
	//chunk->sections.resize(sectionCount);

	//biome and section creation
	int biomeId = 0;

	//sectionMask
	//chunk->sectionMask = new BitArray(sectionCount, 1);

	//light data
	//chunk->lightData.resize((ull)sectionCount + 2);
	//chunk->skyLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->blockLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->emptySkyLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->emptyBlockLightMask = new BitArray(sectionCount + 2, 1);
	//section below the world
	{
		//the section below the world
		chunk->emptySkyLightMask->setElement(0, 1);
		chunk->emptyBlockLightMask->setElement(0, 1);
		chunk->lightSections[0].skyLight = new BitArray(4096, 4);
		chunk->lightSections[0].blockLight = new BitArray(4096, 4);
	}
	//section above the world
	{
		//the section above the world
		BitArray* lightData = new BitArray(4096, 4);
		blong* temp = lightData->getCompactedValues();
		for (int i = 0; i < 256; i++) temp[i] = 0xffffffffffffffff;
		chunk->emptyBlockLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->skyLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->lightSections[(ull)sectionCount + 1].skyLight = lightData;
		chunk->lightSections[(ull)sectionCount + 1].blockLight = new BitArray(4096, 4);
	}
	//[z][x]
	uint heightmaps[16][16]{};
	for (Byte i = 0; i < 16; i++) for (Byte j = 0; j < 16; j++)
	{
		heightmaps[j][i] = int(simplex::get_orig(((X << 4) + i) * noiseFactor_x, ((Z << 4) + j) * noiseFactor_z) * terrainHeightAmplitude + 85 - world->min_y);
		chunk->heightmaps->setElement(j >> 4 | i, heightmaps[j][i]);
	}

	//blocks
	for (uint i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightSections[(ull)i + 1];

		//biomes
		for (int b = 0; b < 64; b++) section.biomes.set(b, biomeId);

		//block data
		int section_base = (i << 4);
		//[y][z][x]
		//Byte blocks[16][16][16]{};
		section.blockCount = 0;

		//[y][z][x]
		bool hasSkyLight = false,
			hasBlockLight = false;
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		//section.palette.emplace_back(PaletteEntry(BlockState("minecraft:air"), 0));
		//section.palette.emplace_back(PaletteEntry(BlockState("minecraft:stone"), 0));

		for (int z = 0; z < 16; z++)
			for (int x = 0; x < 16; x++)
			{
				int adjustedHeight = heightmaps[z][x] - section_base;
				if (adjustedHeight > 16) adjustedHeight = 16;
				else if (adjustedHeight < 0) adjustedHeight = 0;

				section.blockCount += adjustedHeight;
				for (int y = 0; y < adjustedHeight; y++)
				{
					//blocks[y][z][x] = 1;
					section.blockStates.set(y << 8 | z << 4 | x, 1);
					blockLight[y][z][x] = 0;
					skyLight[y][z][x] = 0;
				}
				for (int y = adjustedHeight; y < 16; y++)
				{
					//blocks[y][z][x] = 0;
					blockLight[y][z][x] = 0;
					skyLight[y][z][x] = 15;
					hasSkyLight = true;
				}
			}

		//section.blockStates = new BitArray(0x1000, 4, (Byte*)blocks);
		if (section.blockCount) chunk->sectionMask->setElement(i, 1);
		if (hasSkyLight) chunk->skyLightMask->setElement((ull)i + 1, 1);
		else chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
		if (hasBlockLight) chunk->blockLightMask->setElement((ull)i + 1, 1);
		else chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		lightSection.blockLight = new BitArray(4096, 4, (Byte*)blockLight);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
	}

	//heightmap generation
	//chunk->heightmaps = new BitArray(256, bitCount(world->height), (uint*)heightmaps);
	//for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++) chunk->heightmaps->setElement((ull)z * 16 + x, 144);

	return chunk;
}
Chunk* World::generate_flat(World* world, int x, int z)
{
	//heightmap generation
	int height = world->height;
	Chunk* chunk = new Chunk(height);
	//chunk->heightmaps = new BitArray(256, bitCount(height));
	for (int z0 = 0; z0 < 16; z0++) for (int x0 = 0; x0 < 16; x0++) chunk->heightmaps->setElement((ull)z0 * 16 + x0, ull(80 - world->min_y));

	int biomeId = (x + z) % 10;
	if (biomeId < 0) biomeId += 10;

	uint sectionCount = height >> 4;
	//chunk->sections.resize(sectionCount);

	//light data initialization
	//chunk->lightData.resize((ull)sectionCount + 2);
	//chunk->skyLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->blockLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->emptySkyLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->emptyBlockLightMask = new BitArray(sectionCount + 2, 1);
	//-1 and +1
	{
		//the section below the world
		chunk->emptySkyLightMask->setElement(0, 1);
		chunk->emptyBlockLightMask->setElement(0, 1);
		chunk->lightSections[0].skyLight = new BitArray(4096, 4);
		chunk->lightSections[0].blockLight = new BitArray(4096, 4);
	}
	{
		//the section above the world
		BitArray* lightData = new BitArray(4096, 4);
		blong* temp = lightData->getCompactedValues();
		for (int i = 0; i < 256; i++) temp[i] = 0xffffffffffffffff;
		chunk->emptyBlockLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->skyLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->lightSections[(ull)sectionCount + 1].skyLight = lightData;
		chunk->lightSections[(ull)sectionCount + 1].blockLight = new BitArray(4096, 4);
	}

	if (rand() < 100) for (uint i = 0; i < sectionCount; i++)
	{
		//water chunk
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightSections[(ull)i + 1];

		//biomes
		//section.biomes = new BitArray(64, World::currentBiomeBitsPerEntry);
		for (int b = 0; b < 64; b++) section.biomes.set(b, biomeId);

		//Byte* blocks = new Byte[16 * 16 * 16];
		//[y][z][x]
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(34), 0xf00));
			//section.palette.emplace_back(PaletteEntry(BlockState(33), 0x100));
			for (int j = 0; j < 256; j++)
			{
				//blocks[j] = 1;
				section.blockStates.set(j, 33);
				skyLight[0][j >> 4][j & 15] = 0;
				blockLight[0][j >> 4][j & 15] = 0;
			}
			for (int j = 256; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 34);
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i < 8)
		{
			//underground
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(34), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 34);
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 8)
		{
			//surface layer
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(34), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 34);
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = ((j >> 8) - 1) > 0 ? ((j >> 8) - 1) : 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else
		{
			//air layer
			section.blockCount = 0x0;
			//section.palette.emplace_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}

		lightSection.blockLight = new BitArray(4096, 4, (Byte*)blockLight);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
		//delete[] blocks;
	}
	else if (rand() < 100) for (uint i = 0; i < sectionCount; i++)
	{
		//lava chunk
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightSections[(ull)i + 1];

		//biomes
		//section.biomes = new BitArray(64, World::currentBiomeBitsPerEntry);
		for (int b = 0; b < 64; b++) section.biomes.set(b, biomeId);

		//Byte* blocks = new Byte[16 * 16 * 16];
		//[y][z][x]
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(50), 0xf00));
			//section.palette.emplace_back(PaletteEntry(BlockState(33), 0x100));
			for (int j = 0; j < 256; j++)
			{
				//blocks[j] = 1;
				section.blockStates.set(j, 33);
				skyLight[0][j >> 4][j & 15] = 0;
				blockLight[0][j >> 4][j & 15] = 0;
			}
			for (int j = 256; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 50);
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i < 8)
		{
			//underground layer
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(50), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 50);
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 8)
		{
			//surface layer
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(50), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 50);
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = ((j >> 8) - 1) > 0 ? ((j >> 8) - 1) : 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 9)
		{
			//above surface layer
			section.blockCount = 0x0;
			//section.palette.emplace_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = (14 - (j >> 8)) > 0 ? (14 - (j >> 8)) : 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else
		{
			//air
			section.blockCount = 0x0;
			//section.palette.emplace_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}

		lightSection.blockLight = new BitArray(4096, 4, (Byte*)blockLight);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
		//delete[] blocks;
	}
	else for (uint i = 0; i < sectionCount; i++)
	{
		//normal chunk
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightSections[(ull)i + 1];

		//biomes
		//section.biomes = new BitArray(64, World::currentBiomeBitsPerEntry);
		for (int b = 0; b < 64; b++) section.biomes.set(b, biomeId);

		//Byte* blocks = new Byte[16 * 16 * 16];
		//[y][z][x]
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(1), 0xf00));
			//section.palette.emplace_back(PaletteEntry(BlockState(33), 0x100));
			for (int j = 0; j < 256; j++)
			{
				//blocks[j] = 1;
				section.blockStates.set(j, 33);
				blockLight[0][(j >> 4) & 15][j & 15] = 0;
				skyLight[0][(j >> 4) & 15][j & 15] = 0;
			}
			for (int j = 256; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 1);
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i < 8)
		{
			//underground layer
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(1), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				section.blockStates.set(j, 1);
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 8)
		{
			//surface layer
			section.blockCount = 0x1000;
			//section.palette.emplace_back(PaletteEntry(BlockState(1), 0xd00));
			//section.palette.emplace_back(PaletteEntry(BlockState(10), 0x200));
			//section.palette.emplace_back(PaletteEntry(BlockState(9), 0x100));
			for (int y0 = 0; y0 < 13; y0++) for (int j = 0; j < 256; j++)
			{
				//blocks[y0 * 256 + j] = 0;
				section.blockStates.set(y0 << 8 | j, 1);
				blockLight[y0][(j >> 4) & 15][j & 15] = 0;
				skyLight[y0][(j >> 4) & 15][j & 15] = 0;
			}
			for (int y0 = 13; y0 < 15; y0++) for (int j = 0; j < 256; j++)
			{
				//blocks[y0 * 256 + j] = 1;
				section.blockStates.set(y0 << 8 | j, 10);
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			for (int j = 0; j < 256; j++)
			{
				//blocks[15 * 16 * 16 + j] = 2;
				section.blockStates.set(15 << 8 | j, 9);
				blockLight[15][(j >> 4) & 15][j & 15] = 0;
				skyLight[15][(j >> 4) & 15][j & 15] = 0;
			}
			//section.blockStates = new BitArray(16 * 16 * 16, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else
		{
			//air layer
			section.blockCount = 0x0;
			//section.palette.emplace_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				//blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			//section.blockStates = new BitArray(4096, 4, blocks);

			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}

		lightSection.blockLight = new BitArray(4096, 4, (Byte*)blockLight);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
		//delete[] blocks;
	}

	return chunk;
}
Chunk* World::generate_void(World* world, int x, int z)
{
	//heightmap generation
	int height = world->height;
	Chunk* chunk = new Chunk((uint)height);
	//chunk->heightmaps = new BitArray(256, bitCount(height));

	int biomeId = 7;

	uint sectionCount = height >> 4;
	//chunk->sections.resize(sectionCount);

	//light data initialization
	//chunk->lightData.resize((ull)sectionCount + 2);
	//chunk->skyLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->blockLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->emptySkyLightMask = new BitArray(sectionCount + 2, 1);
	//chunk->emptyBlockLightMask = new BitArray(sectionCount + 2, 1);
	//-1 and +1
	{
		BitArray* lightData = new BitArray(4096, 4);
		blong* temp = lightData->getCompactedValues();
		for (int i = 0; i < 256; i++) temp[i] = 0xffffffffffffffff;
		//the section below the world
		chunk->skyLightMask->setElement(0, 1);
		chunk->emptyBlockLightMask->setElement(0, 1);
		chunk->lightSections[0].skyLight = lightData;
		chunk->lightSections[0].blockLight = new BitArray(4096, 4);
	}
	{
		//the section above the world
		BitArray* lightData = new BitArray(4096, 4);
		blong* temp = lightData->getCompactedValues();
		for (int i = 0; i < 256; i++) temp[i] = 0xffffffffffffffff;
		chunk->emptyBlockLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->skyLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->lightSections[(ull)sectionCount + 1].skyLight = lightData;
		chunk->lightSections[(ull)sectionCount + 1].blockLight = new BitArray(4096, 4);
	}

	//primary mask initialization
	//chunk->sectionMask = new BitArray(sectionCount, 1);

	for (uint i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightSections[(ull)i + 1];

		//biomes
		//section.biomes = new BitArray(64, World::currentBiomeBitsPerEntry);
		for (int b = 0; b < 64; b++) section.biomes.set(b, biomeId);

		//[y][z][x]
		Byte skyLight[16][16][16]{};

		//data
		section.blockCount = 0x0;
		for (int j = 0; j < 4096; j++)
		{
			skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
		}
		//section.blockStates = new BitArray(4096, 4);

		chunk->skyLightMask->setElement((ull)i + 1, 1);
		chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);

		lightSection.blockLight = new BitArray(4096, 4);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
	}

	return chunk;
}

void World::unload(int x, int z)
{
	int rX = x >> 5,
		rZ = z >> 5,
		relX = x & 31,
		relZ = z & 31;

	//std::cout << "Chunk [" << x << ", " << z << "] unloaded\n";

	for (ull i = 0; i < regions.size(); i++) if (rX == regions[i]->rX && rZ == regions[i]->rZ)
	{
		Region*& region = regions[i];
		region->unload(this, relX, relZ);
		if (!region->hasChunksLoaded())
		{
			delete regions[i];
			regions.erase(regions.begin() + i);
			Log::debug(REGION_DEBUG) << "Regions is now " << regions.size() << Log::endl;
		}
		return;
	}

	Log::info() << "Incorrect chunk unload at [" << x << ", " << z << "]" << Log::endl;
	throw runtimeWarning("Tried to unload a chunk in an unloaded region");
}
Chunk* World::getChunk(int x, int z, bool increaseLoadCount)
{
	//region coordinates and relative chunk coordinates
	int rX = x >> 5,
		rZ = z >> 5,
		relX = x & 31,
		relZ = z & 31;

	//search for region
	for (ull i = 0; i < regions.size(); i++) if (rX == regions[i]->rX && rZ == regions[i]->rZ)
	{
		//region found, extract the chunk
		Chunk* chunk = regions[i]->get(this, relX, relZ, increaseLoadCount);
		if (chunk)
		{
			Log::debug(REGION_DEBUG) << "Chunk [" << x << ", " << z << "] extracted(" << chunk->loadCount << ")" << Log::flush;
			return chunk;
		}
		//chunk not found in region, generate
		if (!increaseLoadCount) return nullptr;
		chunk = generatorFunction(this, x, z);
		chunk->loadCount = 1;
		Log::debug(REGION_DEBUG) << "Chunk [" << x << ", " << z << "] generated(" << chunk->loadCount << ")" << Log::flush;
		regions[i]->set(relX, relZ, chunk);
		return chunk;
	}

	//region not found, create region and load chunk
	if (!increaseLoadCount) return nullptr;
	Region* region = new Region(name, rX, rZ);
	regions.emplace_back(region);
	Log::debug(REGION_DEBUG) << "Regions is now " << regions.size() << Log::flush;
	//try to load the chunk from the region
	Chunk* chunk = region->get(this, relX, relZ, increaseLoadCount);
	if (chunk)
	{
		Log::debug(REGION_DEBUG) << "Chunk [" << x << ", " << z << "] extracted(" << chunk->loadCount << ")" << Log::flush;
		return chunk;
	}
	//chunk could not be loaded, generate
	chunk = generatorFunction(this, x, z);
	chunk->loadCount = 1;
	Log::debug(REGION_DEBUG) << "Chunk [" << x << ", " << z << "] generated(" << chunk->loadCount << ")" << Log::flush;

	region->set(relX, relZ, chunk);
	return chunk;
}
Region* World::getRegion(int rX, int rZ)
{
	for (Region* reg : regions) if (rX == reg->rX && rZ == reg->rZ) return reg;
	return nullptr;
}
//-1 for blocks outside loaded area
int World::getBlock(int x, int y, int z)
{
	int rX = x >> 9,
		rZ = z >> 9;
	x &= 0x1ff; z &= 0x1ff;

	Region* reg = getRegion(rX, rZ);
	if (reg)
	{
		return reg->getBlock(x, y, z);
	}
	return -1;
}
bool World::setBlock(int x, int y, int z, int blockid, nbt_compound* nbt_data, Player* broadcastException)
{
	Position loc(x, y + min_y, z);
	bool ret = setBlockNoBroadcast(x, y, z, blockid, nbt_data);
	for (Player* p : players) if (p != broadcastException && p->positionInRange(loc)) message::play::send::blockChange(p, loc, blockid);
	return ret;
}
bool World::setBlockNoBroadcast(int x, int y, int z, int blockid, nbt_compound* nbt_data)
{
	int rX = x >> 9,
		rZ = z >> 9;
	x &= 0x1ff; z &= 0x1ff;

	Region* reg = getRegion(rX, rZ);
	if (!reg) genException("World::setBlock: region not loaded");
	return reg->setBlock(x, y, z, blockid, nbt_data);
}

Byte World::getSkyLight(int x, int y, int z)
{
	Region* reg = getRegion(x >> 9, z >> 9);
	if (!reg)
	{
		//if the region is not loaded, return 0xff for now
		return 0xff;
	}
	return reg->getSkyLight(x & 0x1ff, y, z & 0x1ff);
}
void World::setSkyLight(int x, int y, int z, Byte value)
{
	Region* reg = getRegion(x >> 9, z >> 9);
	if (!reg)
	{
		//if the region is not loaded, do nothing for now
		return;
	}
	reg->setSkyLight(x & 0x1ff, y, z & 0x1ff, value);
}
Byte World::getBlockLight(int x, int y, int z)
{
	Region* reg = getRegion(x >> 9, z >> 9);
	if (!reg)
	{
		//if the region is not loaded, return 0xff for now
		return 0xff;
	}
	return reg->getBlockLight(x & 0x1ff, y, z & 0x1ff);
}
void World::setBlockLight(int x, int y, int z, Byte value)
{
	Region* reg = getRegion(x >> 9, z >> 9);
	if (!reg)
	{
		//if the region is not loaded, do nothing for now
		return;
	}
	reg->setBlockLight(x & 0x1ff, y, z & 0x1ff, value);
}

void World::setLightSource(int x, int y, int z, Byte sourceLight)
{
	//wip
}
void World::destroyLightSource(int x, int y, int z)
{
	//wip
}

Byte calculateLight()
{
	return 0;
}
Byte World::updateLightAux(int x, int y, int z, const BlockState* state)
{
	//from +y


	//from -y

	//from +x

	//from -x

	//from +z

	//from -z
	return 0;
}
void World::updateLight(int x, int y, int z)
{
	const BlockState* state = BlockState::globalPalette[getBlock(x, y, z)];
	//Blocks::Transparency transparency = state->getTransparency();
	updateLightAux(x, y, z, state);

	//update block light


	//update sky light

}

bool World::loadAll()
{
	ifstream worldList("worlds\\worldList.txt");
	std::string name;
	static std::mutex loader;
	std::vector<std::future<World*>> futures;
	while (worldList >> name)
	{
		futures.emplace_back(Server::threadPool.enqueue([name]
		{
			Log::Bench(name);
			World *zaWarudo;
			try
			{
				zaWarudo = new World(name.c_str());

			loader.lock();
			Log::info() << "thread-" << std::this_thread::get_id() << " loaded \"" << name << "\" " << Log::Bench(name) << Log::endl;
			loader.unlock();
			}
			catch (mcException &e)
			{
				zaWarudo = nullptr;

				loader.lock();
				Log::error() << e.what() << Log::endl;
				Log::info() << "thread-" << std::this_thread::get_id() << " failed to load \"" << name << "\" " << Log::Bench(name) << Log::endl;
				loader.unlock();
			}
			return zaWarudo;
		}));
	}

	ull worldCount = 0;
	for (auto&& fut : futures)
	{
		World *wld = fut.get();
		if (wld)
		{
			worlds.emplace_front(wld);
			worldCount++;
		}
	}

	Log::info() << "Successfully loaded " << worldCount << " worlds! " << Log::Bench("worlds") << Log::flush;
	worldList.close();

	spawnWorld = getWorld(Options::mainWorldName());
	return spawnWorld != nullptr;
}
void World::unloadAll()
{
	for (World* w : worlds) delete w;
	worlds.clear();
}

World* World::getWorld(const mcString& worldName)
{
	for (World* w : worlds) if (worldName == w->name) return w;
	return nullptr;
}

void World::tick()
{
	for (Region* reg : regions) reg->tick(this, randomTickSpeed);
}

string precToString(World::Biome::Precipitation prec)
{
	switch (prec)
	{
	case World::Biome::Precipitation::rain:
		return "rain";
	case World::Biome::Precipitation::snow:
		return "snow";
	default:
		return "none";
	}
}
string categoryToString(World::Biome::Category category)
{
	switch (category)
	{
	case World::Biome::Category::ocean:
		return "ocean";
	case World::Biome::Category::plains:
		return "plains";
	case World::Biome::Category::desert:
		return "ddesert";
	case World::Biome::Category::forest:
		return "forest";
	case World::Biome::Category::extreme_hills:
		return "extreme_hills";
	case World::Biome::Category::taiga:
		return "taiga";
	case World::Biome::Category::swamp:
		return "swamp";
	case World::Biome::Category::river:
		return "river";
	case World::Biome::Category::nether:
		return "nether";
	case World::Biome::Category::the_end:
		return "the_end";
	case World::Biome::Category::icy:
		return "icy";
	case World::Biome::Category::mushroom:
		return "mushroom";
	case World::Biome::Category::beach:
		return "beach";
	case World::Biome::Category::jungle:
		return "jungle";
	case World::Biome::Category::mesa:
		return "mesa";
	case World::Biome::Category::savana:
		return "savana";
	default:
		return "none";
	}
}

nbt_compound* World::Biome::getNbt(int id, const std::string& name, Precipitation prec, float depth, float temperature, float scale, float downfall, Category category, Color skyColor, Color waterFogColor, Color fogColor, Color waterColor, Color foliageColor, Color grassColor)
{
	return new nbt_compound("", new nbt * [3]{
				new nbt_string("name", name),
				new nbt_int("id",8),
				new nbt_compound("element",new nbt * [7]{
					new nbt_string("precipitation", precToString(prec)),
					new nbt_float("depth", depth),
					new nbt_float("temperature", temperature),
					new nbt_float("scale", scale),
					new nbt_float("downfall", downfall),
					new nbt_string("category", categoryToString(category)),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color", skyColor.value),
						new nbt_int("water_fog_color", waterFogColor.value),
						new nbt_int("fog_color", fogColor.value),
						new nbt_int("water_color", waterColor.value),
						new nbt_int("foliage_color", foliageColor.value),
						new nbt_int("grass_color", grassColor.value)
					},6)
				},7)
		}, 3);
}