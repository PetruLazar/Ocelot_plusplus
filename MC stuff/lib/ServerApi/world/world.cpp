#include "../world.h"
#include "../types/utils.h"
#include <iostream>
#include "noise.h"
#include "../types/error.h"
#include "../types/basic.h"
#include "../server/log.h"
#include "../server/options.h"
#include "../player/message.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

const char invalidMainFile[] = "Invalid characteristics.bin file.";
const char noAccesMainFile[] = "Could not open characteristics.bin file";
const ull levelDatMaxSize = 1024 * 1024; //size after decompression

const int terrainHeightAverage = 85;
const int terrainHeightAmplitude = 35;
const double noiseFactor_x = 1. / 128, noiseFactor_z = 1. / 128;

std::vector<World*> World::worlds;
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
					new nbt_string("infiniburn","minecraft:infiniburn_overworld"),
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
					new nbt_string("infiniburn",""),
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
/*nbt_compound World::dimension("", new nbt* [15]{
						new nbt_Byte("piglin_safe",0),
						new nbt_Byte("natural",1),
						new nbt_float("ambient_light",0.f),
						new nbt_string("infiniburn","minecraft:infiniburn_overworld"),
						new nbt_Byte("respawn_anchor_works",0),
						new nbt_Byte("has_skylight",1),
						new nbt_Byte("bed_works",1),
						new nbt_string("effects","minecraft:overworld"),
						new nbt_Byte("has_raids",1),
						new nbt_int("min_y",-64),
						new nbt_int("height",384),
						new nbt_int("logical_height",256),
						new nbt_float("coordinate_scale",1.f),
						new nbt_Byte("ultrawarm",0),
						new nbt_Byte("has_ceiling",0)
	}, 15);*/

World::World(const char* c_name) : name(c_name), characteristics("", nullptr)
{
	IF_WORLD_LOAD_DEBUG(Log::txt() << "\nLoading world \"" << c_name << "\"..." << Log::flush);
	fstream worldMain("worlds\\" + name + "\\characteristics.bin", ios::binary | ios::in);
	if (!worldMain.is_open())
	{
		Log::txt() << "Error: cannot open charactestics.bin\n" << Log::flush;
		throw 0;
	}
	if (!nbt::checkTag(worldMain))
	{
		Log::txt() << "Error: charactestics.bin has an invalid format\n" << Log::flush;
		throw 0;
	}
	characteristics.read(worldMain);
	height = characteristics["height"].vInt();
	min_y = characteristics["min_y"].vInt();

	spawn.X.read(worldMain);
	spawn.Z.read(worldMain);
	spawn.Yaw.read(worldMain);
	spawn.Pitch.read(worldMain);

	spawn.ChunkX = int(floor(spawn.X)) >> 4;
	spawn.ChunkZ = int(floor(spawn.Z)) >> 4;
	spawn.Absolute = sf::Vector3i(int(floor(spawn.X)), int(floor(spawn.Y)), int(floor(spawn.Z)));

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
		catch (...)
		{
			Log::txt() << "\nCould not load custom generator for world \"" << c_name << "\", using default instead." << Log::flush;
			generatorFunction = generate_def;
		}
		break;
	}

	//if regions folder is absent, create it
	//fs::path regionsFolder();
	if (!fs::is_directory("worlds\\" + name + "\\regions"))
	{
		fs::create_directory("worlds\\" + name + "\\regions");
		Log::txt() << "\nCreated \"regions\" directory for world " << name << Log::flush;
	}

	IF_WORLD_LOAD_DEBUG(Log::txt() << "\nLoading spawn area..." << Log::flush);
	for (int x = spawn.ChunkX - Options::viewDistance(); x <= spawn.ChunkX + Options::viewDistance(); x++)
		for (int z = spawn.ChunkZ - Options::viewDistance(); z <= spawn.ChunkZ + Options::viewDistance(); z++)
			get(x, z, true);

	spawn.Y = double(characteristics["min_y"].vInt()) + get(spawn.ChunkX, spawn.ChunkZ)->heightmaps->getElement(((ull)spawn.Absolute.z() - ((ull)spawn.ChunkZ << 4)) * 16 + ((ull)spawn.Absolute.x() - ((ull)spawn.ChunkX << 4)));
	IF_WORLD_LOAD_DEBUG(Log::txt() << "\nDone!" << Log::flush);
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
		chunk->lightData[0].skyLight = new BitArray(4096, 4);
		chunk->lightData[0].blockLight = new BitArray(4096, 4);
	}
	//section above the world
	{
		//the section above the world
		BitArray* lightData = new BitArray(4096, 4);
		blong* temp = lightData->getCompactedValues();
		for (int i = 0; i < 256; i++) temp[i] = 0xffffffffffffffff;
		chunk->emptyBlockLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->skyLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->lightData[(ull)sectionCount + 1].skyLight = lightData;
		chunk->lightData[(ull)sectionCount + 1].blockLight = new BitArray(4096, 4);
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
		LightSection& lightSection = chunk->lightData[(ull)i + 1];

		//biomes
		for (int y = 0; y < 4; y++) for (int z = 0; z < 4; z++) for (int x = 0; x < 4; x++) section.biomes[y][z][x] = biomeId;

		//block data
		int section_base = (i << 4);
		//[y][z][x]
		Byte blocks[16][16][16]{};
		section.blockCount = 0;
		section.bitsPerBlock = 4;
		section.useGlobalPallete = false;

		//[y][z][x]
		bool hasSkyLight = false,
			hasBlockLight = false;
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		section.palette.push_back(PaletteEntry(BlockState("minecraft:air"), 0));
		section.palette.push_back(PaletteEntry(BlockState("minecraft:stone"), 0));

		for (int z = 0; z < 16; z++)
			for (int x = 0; x < 16; x++)
			{
				int adjustedHeight = heightmaps[z][x] - section_base;
				if (adjustedHeight > 16) adjustedHeight = 16;
				else if (adjustedHeight < 0) adjustedHeight = 0;

				//if (i == 9 && z == 3 && x == 4)
				//	cout << ' ';

				section.blockCount += adjustedHeight;
				for (int y = 0; y < adjustedHeight; y++)
				{
					blocks[y][z][x] = 1;
					blockLight[y][z][x] = 0;
					skyLight[y][z][x] = 0;
				}
				for (int y = adjustedHeight; y < 16; y++)
				{
					blocks[y][z][x] = 0;
					blockLight[y][z][x] = 0;
					skyLight[y][z][x] = 15;
					hasSkyLight = true;
				}
			}

		section.palette[1].referenceCount = section.blockCount;
		section.palette[0].referenceCount = 4096 - section.blockCount;
		section.blockStates = new BitArray(0x1000, 4, (Byte*)blocks);
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
		chunk->lightData[0].skyLight = new BitArray(4096, 4);
		chunk->lightData[0].blockLight = new BitArray(4096, 4);
	}
	{
		//the section above the world
		BitArray* lightData = new BitArray(4096, 4);
		blong* temp = lightData->getCompactedValues();
		for (int i = 0; i < 256; i++) temp[i] = 0xffffffffffffffff;
		chunk->emptyBlockLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->skyLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->lightData[(ull)sectionCount + 1].skyLight = lightData;
		chunk->lightData[(ull)sectionCount + 1].blockLight = new BitArray(4096, 4);
	}

	if (rand() < 100) for (uint i = 0; i < sectionCount; i++)
	{
		//water chunk
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightData[(ull)i + 1];

		//biomes
		for (int x0 = 0; x0 < 4; x0++) for (int z0 = 0; z0 < 4; z0++) for (int y0 = 0; y0 < 4; y0++) section.biomes[x0][y0][z0] = biomeId;

		Byte* blocks = new Byte[16 * 16 * 16];
		//[y][z][x]
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(34), 0xf00));
			section.palette.push_back(PaletteEntry(BlockState(33), 0x100));
			for (int j = 0; j < 256; j++)
			{
				blocks[j] = 1;
				skyLight[0][j >> 4][j & 15] = 0;
				blockLight[0][j >> 4][j & 15] = 0;
			}
			for (int j = 256; j < 4096; j++)
			{
				blocks[j] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i < 8)
		{
			//underground
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(34), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 8)
		{
			//surface layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(34), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = ((j >> 8) - 1) > 0 ? ((j >> 8) - 1) : 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else
		{
			//air layer
			section.blockCount = 0x0;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}

		lightSection.blockLight = new BitArray(4096, 4, (Byte*)blockLight);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
		delete[] blocks;
	}
	else if (rand() < 100) for (uint i = 0; i < sectionCount; i++)
	{
		//lava chunk
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightData[(ull)i + 1];

		//biomes
		for (int x0 = 0; x0 < 4; x0++) for (int z0 = 0; z0 < 4; z0++) for (int y0 = 0; y0 < 4; y0++) section.biomes[x0][y0][z0] = biomeId;

		Byte* blocks = new Byte[16 * 16 * 16];
		//[y][z][x]
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(50), 0xf00));
			section.palette.push_back(PaletteEntry(BlockState(33), 0x100));
			for (int j = 0; j < 256; j++)
			{
				blocks[j] = 1;
				skyLight[0][j >> 4][j & 15] = 0;
				blockLight[0][j >> 4][j & 15] = 0;
			}
			for (int j = 256; j < 4096; j++)
			{
				blocks[j] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i < 8)
		{
			//underground layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(50), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 8)
		{
			//surface layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(50), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = ((j >> 8) - 1) > 0 ? ((j >> 8) - 1) : 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 9)
		{
			//above surface layer
			section.blockCount = 0x0;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = (14 - (j >> 8)) > 0 ? (14 - (j >> 8)) : 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			section.blockStates = new BitArray(4096, 4, blocks);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->blockLightMask->setElement((ull)i + 1, 1);
		}
		else
		{
			//air
			section.blockCount = 0x0;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			section.blockStates = new BitArray(4096, 4, blocks);
			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}

		lightSection.blockLight = new BitArray(4096, 4, (Byte*)blockLight);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
		delete[] blocks;
	}
	else for (uint i = 0; i < sectionCount; i++)
	{
		//normal chunk
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightData[(ull)i + 1];

		//biomes
		for (int x0 = 0; x0 < 4; x0++) for (int z0 = 0; z0 < 4; z0++) for (int y0 = 0; y0 < 4; y0++) section.biomes[x0][y0][z0] = biomeId;

		Byte* blocks = new Byte[16 * 16 * 16];
		//[y][z][x]
		Byte skyLight[16][16][16]{},
			blockLight[16][16][16]{};

		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(1), 0xf00));
			section.palette.push_back(PaletteEntry(BlockState(33), 0x100));
			for (int j = 0; j < 256; j++)
			{
				blocks[j] = 1;
				blockLight[0][(j >> 4) & 15][j & 15] = 0;
				skyLight[0][(j >> 4) & 15][j & 15] = 0;
			}
			for (int j = 256; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i < 8)
		{
			//underground layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(1), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else if (i == 8)
		{
			//surface layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(1), 0xd00));
			section.palette.push_back(PaletteEntry(BlockState(10), 0x200));
			section.palette.push_back(PaletteEntry(BlockState(9), 0x100));
			for (int y0 = 0; y0 < 13; y0++) for (int j = 0; j < 256; j++)
			{
				blocks[y0 * 256 + j] = 0;
				blockLight[y0][(j >> 4) & 15][j & 15] = 0;
				skyLight[y0][(j >> 4) & 15][j & 15] = 0;
			}
			for (int y0 = 13; y0 < 15; y0++) for (int j = 0; j < 256; j++)
			{
				blocks[y0 * 256 + j] = 1;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
			}
			for (int j = 0; j < 256; j++)
			{
				blocks[15 * 16 * 16 + j] = 2;
				blockLight[15][(j >> 4) & 15][j & 15] = 0;
				skyLight[15][(j >> 4) & 15][j & 15] = 0;
			}
			section.blockStates = new BitArray(16 * 16 * 16, 4, blocks);

			chunk->sectionMask->setElement(i, 1);
			chunk->emptySkyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}
		else
		{
			//air layer
			section.blockCount = 0x0;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.palette.push_back(PaletteEntry(BlockState(0), 0x1000));
			for (int j = 0; j < 4096; j++)
			{
				blocks[j] = 0;
				blockLight[j >> 8][(j >> 4) & 15][j & 15] = 0;
				skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
			}
			section.blockStates = new BitArray(4096, 4, blocks);

			chunk->skyLightMask->setElement((ull)i + 1, 1);
			chunk->emptyBlockLightMask->setElement((ull)i + 1, 1);
		}

		lightSection.blockLight = new BitArray(4096, 4, (Byte*)blockLight);
		lightSection.skyLight = new BitArray(4096, 4, (Byte*)skyLight);
		delete[] blocks;
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
		chunk->lightData[0].skyLight = lightData;
		chunk->lightData[0].blockLight = new BitArray(4096, 4);
	}
	{
		//the section above the world
		BitArray* lightData = new BitArray(4096, 4);
		blong* temp = lightData->getCompactedValues();
		for (int i = 0; i < 256; i++) temp[i] = 0xffffffffffffffff;
		chunk->emptyBlockLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->skyLightMask->setElement((ull)sectionCount + 1, 1);
		chunk->lightData[(ull)sectionCount + 1].skyLight = lightData;
		chunk->lightData[(ull)sectionCount + 1].blockLight = new BitArray(4096, 4);
	}

	//primary mask initialization
	//chunk->sectionMask = new BitArray(sectionCount, 1);

	for (uint i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];
		LightSection& lightSection = chunk->lightData[(ull)i + 1];

		//biomes
		for (int x0 = 0; x0 < 4; x0++) for (int z0 = 0; z0 < 4; z0++) for (int y0 = 0; y0 < 4; y0++) section.biomes[x0][y0][z0] = biomeId;

		//[y][z][x]
		Byte skyLight[16][16][16]{};

		//data
		section.blockCount = 0x0;
		section.bitsPerBlock = 4;
		section.useGlobalPallete = false;
		section.palette.push_back(PaletteEntry(BlockState(0), 0x1000));
		for (int j = 0; j < 4096; j++)
		{
			skyLight[j >> 8][(j >> 4) & 15][j & 15] = 15;
		}
		section.blockStates = new BitArray(4096, 4);

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
			regions.erase(regions.begin() + i);
			IF_REGION_DEBUG(std::cout << "\nRegions is now " << regions.size());
		}
		return;
	}

	std::cout << "\nIncorrect chunk unload at [" << x << ", " << z << "]";
	throw runtimeWarning("Tried to unload a chunk in an unloaded region");
}
Chunk* World::get(int x, int z, bool increaseLoadCount)
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
			IF_CHUNK_DEBUG(Log::txt() << "\nChunk [" << x << ", " << z << "] extracted(" << chunk->loadCount << ")" << Log::flush;);
			return chunk;
		}
		//chunk not found in region, generate
		chunk = generatorFunction(this, x, z);
		chunk->loadCount = 1;
		IF_CHUNK_DEBUG(Log::txt() << "\nChunk [" << x << ", " << z << "] generated(" << chunk->loadCount << ")" << Log::flush;);
		regions[i]->set(relX, relZ, chunk);
		return chunk;
	}

	//region not found, create region and load chunk
	Region* region = new Region(name, rX, rZ);
	regions.push_back(region);
	IF_REGION_DEBUG(Log::txt() << "\nRegions is now " << regions.size() << Log::flush;);
	//try to load the chunk from the region
	Chunk* chunk = region->get(this, relX, relZ, increaseLoadCount);
	if (chunk)
	{
		IF_CHUNK_DEBUG(Log::txt() << "\nChunk [" << x << ", " << z << "] extracted(" << chunk->loadCount << ")" << Log::flush;);
		return chunk;
	}
	//chunk could not be loaded, generate
	chunk = generatorFunction(this, x, z);
	chunk->loadCount = 1;
	IF_CHUNK_DEBUG(Log::txt() << "\nChunk [" << x << ", " << z << "] generated(" << chunk->loadCount << ")" << Log::flush;);
	region->set(relX, relZ, chunk);
	return chunk;
}
Region* World::getRegion(int rX, int rZ)
{
	for (Region* reg : regions) if (rX == reg->rX && rZ == reg->rZ) return reg;
	return nullptr;
}

int World::AbsToRelHeight(int y)
{
	return y - min_y;
}
bool World::checkCoordinates(int y)
{
	return y >= 0 && y < height;

}
BlockState& World::getPaletteEntry(int x, int y, int z)
{
	int rX = x >> 9,
		rZ = z >> 9;
	x &= 0x1ff; z &= 0x1ff;

	Region* reg = getRegion(rX, rZ);
	if (reg)
	{
		return reg->getPaletteEntry(x, y, z);
	}
	throw std::exception("World::getBlock: region not loaded");
}
BlockState& World::getPaletteEntry(int cx, int cy, int cz, int paletteIndex)
{
	int rX = cx >> 5,
		rZ = cz >> 5;
	cx &= 0x1f; cz &= 0x1f;

	Region* reg = getRegion(rX, rZ);
	if (reg)
	{
		return reg->getPaletteEntry(cx, cy, cz, paletteIndex);
	}
	throw std::exception("World::getBlock: region not loaded");
}
BlockState World::getBlock(int x, int y, int z)
{
	int rX = x >> 9,
		rZ = z >> 9;
	x &= 0x1ff; z &= 0x1ff;

	Region* reg = getRegion(rX, rZ);
	if (reg)
	{
		return reg->getBlock(x, y, z);
	}
	throw std::exception("World::getBlock: region not loaded");
}
void World::setBlock(int x, int y, int z, const BlockState& bl, Player* broadcastException)
{
	Position loc(x, y + min_y, z);

	int rX = x >> 9,
		rZ = z >> 9;
	x &= 0x1ff; z &= 0x1ff;

	Region* reg = getRegion(rX, rZ);
	if (!reg) throw std::exception("World::setBlock: region not loaded");
	reg->setBlock(x, y, z, bl);

	for (Player* p : players) if (p != broadcastException && p->positionInRange(loc)) message::play::send::blockChange(p, loc, bl.id);

	//setBlock(destX, destY, destZ, stateJson);
	//Position destLoc = Position(destX, destY + p->world->min_y, destZ);
	//for (Player* seener : players) if (seener != p && seener->positionInRange(destLoc)) message::play::send::blockChange(seener, destLoc, (*stateJson)["id"].iValue());
}
void World::setBlockNoBroadcast(int x, int y, int z, const BlockState& bl)
{
	int rX = x >> 9,
		rZ = z >> 9;
	x &= 0x1ff; z &= 0x1ff;

	Region* reg = getRegion(rX, rZ);
	if (!reg) throw std::exception("World::setBlock: region not loaded");
	reg->setBlock(x, y, z, bl);
}

bool World::loadAll()
{
	ifstream worldList("worlds\\worldList.txt");
	char name[256];
	while (worldList >> name)
	{
		worlds.push_back(new World(name));
	}
	Log::txt() << "\nFinished loading " << worlds.size() << " worlds!" << Log::flush;
	worldList.close();

	spawnWorld = getWorld(Options::mainWorldName());
	return spawnWorld != nullptr;
}
void World::unloadAll()
{
	for (World* w : worlds) delete w;
}

World* World::getWorld(const mcString& worldName)
{
	for (World* w : worlds) if (worldName == w->name) return w;
	return nullptr;
}