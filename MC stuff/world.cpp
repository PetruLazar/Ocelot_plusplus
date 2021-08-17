#include "world.h"

using namespace std;

const char invalidMainFile[] = "Invalid characteristics.bin file.";
const char noAccesMainFile[] = "Could not open characteristics.bin file";
const ull levelDatMaxSize = 1024 * 1024; //size after decompression

std::vector<World*> World::worlds;
nbt_compound World::dimension_codec("", new nbt* [2]{
	new nbt_compound("minecraft:dimension_type",new nbt * [2]{
		new nbt_string("type","minecraft:dimension_type"),
		new nbt_list("value",new nbt * [2]{
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:overworld"),
				new nbt_int("id",0),
				new nbt_compound("element",new nbt * [15]{
					new nbt_byte("piglin_safe",0),
					new nbt_byte("natural",1),
					new nbt_float("ambient_light",0.f),
					new nbt_string("infiniburn","minecraft:infiniburn_overworld"),
					new nbt_byte("respawn_anchor_works",0),
					new nbt_byte("has_skylight",1),
					new nbt_byte("bed_works",1),
					new nbt_string("effects","minecraft:overworld"),
					new nbt_byte("has_raids",1),
					new nbt_int("min_y",0),
					new nbt_int("height",256),
					new nbt_int("logical_height",256),
					new nbt_float("coordinate_scale",1.f),
					new nbt_byte("ultrawarm",0),
					new nbt_byte("has_ceiling",0)
				},15)
			},3),
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:fakeworld"),
				new nbt_int("id",1),
				new nbt_compound("element",new nbt * [15]{
					new nbt_byte("piglin_safe",1),
					new nbt_byte("natural",0),
					new nbt_float("ambient_light",1.f),
					new nbt_string("infiniburn",""),
					new nbt_byte("respawn_anchor_works",0),
					new nbt_byte("has_skylight",1),
					new nbt_byte("bed_works",0),
					new nbt_string("effects","minecraft:the_end"),
					new nbt_byte("has_raids",0),
					new nbt_int("min_y",0),
					new nbt_int("height",256),
					new nbt_int("logical_height",256),
					new nbt_float("coordinate_scale",1.f),
					new nbt_byte("ultrawarm",0),
					new nbt_byte("has_ceiling",0)
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
					new nbt_string("precipitation","none"),
					new nbt_float("depth", 0.f),
					new nbt_float("temperature", 1.f),
					new nbt_float("scale", .75f),
					new nbt_float("downfall", .5f),
					new nbt_string("category", "none"),
					new nbt_compound("effects",new nbt * [6] {
						new nbt_int("sky_color",0x00),
						new nbt_int("water_fog_color",0x00),
						new nbt_int("fog_color",0x00),
						new nbt_int("water_color",0x00),
						new nbt_int("foliage_color",0x00),
						new nbt_int("grass_color",0x00)
					},6)
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
						new nbt_byte("piglin_safe",0),
						new nbt_byte("natural",1),
						new nbt_float("ambient_light",0.f),
						new nbt_string("infiniburn","minecraft:infiniburn_overworld"),
						new nbt_byte("respawn_anchor_works",0),
						new nbt_byte("has_skylight",1),
						new nbt_byte("bed_works",1),
						new nbt_string("effects","minecraft:overworld"),
						new nbt_byte("has_raids",1),
						new nbt_int("min_y",-64),
						new nbt_int("height",384),
						new nbt_int("logical_height",256),
						new nbt_float("coordinate_scale",1.f),
						new nbt_byte("ultrawarm",0),
						new nbt_byte("has_ceiling",0)
	}, 15);*/
	/*nbt_compound World::heightMap("", new nbt* [1]{
		new nbt_long_array("MOTION_BLOCKING",new int64[37]{0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64,0b1100000011100000011100000011100000011100000011100000011100000010i64},37)
		}, 1);*/

World::World(const char* c_name) : name(c_name), characteristics("", nullptr)
{
	cout << "Loading world \"" << c_name << "...\n";
	fstream worldMain("worlds\\" + name + "\\characteristics.bin", ios::binary | ios::in);
	if (!worldMain.is_open())
	{
		cout << "Error: cannot open charactestics.bin\n";
		throw 0;
	}
	if (!nbt::checkTag(worldMain))
	{
		cout << "Error: charactestics.bin has an invalid format\n";
		throw 0;
	}
	characteristics.read(worldMain);
	spawnX.read(worldMain);
	spawnZ.read(worldMain);
	spawnYaw.read(worldMain);
	spawnPitch.read(worldMain);

	spawnChunkX = fastfloor(spawnX) >> 4;
	spawnChunkZ = fastfloor(spawnZ) >> 4;
	spawn = sf::Vector3i(fastfloor(spawnX), fastfloor(spawnY), fastfloor(spawnZ));

	cout << "Loading spawn area...\n";
	for (int x = spawnChunkX - 3; x < spawnChunkX + 4; x++) for (int z = spawnChunkZ - 3; x < spawnChunkZ + 4; x++) get(x, z)->loadCount = 1;

	spawnY = double(characteristics["min_y"].vInt()) + get(spawnChunkX, spawnChunkZ)->heightmaps->getElement(((ull)spawn.z() - ((ull)spawnChunkZ << 4)) * 16 + ((ull)spawn.x() - ((ull)spawnChunkX << 4)));
	cout << "Done!\n";
}
World::~World()
{
	for (Region* r : regions)
	{
		r->unload(this);
		delete r;
	}
	//update characteristics.bin
}

Chunk* World::generate(int x, int z)
{
	//predefined chunks for now
	Chunk* chunk = new Chunk;

	//heightmap generation
	int height = characteristics["height"].vInt();
	chunk->heightmaps = new BitArray(256, bitCount(height));
	for (int z0 = 0; z0 < 16; z0++) for (int x0 = 0; x0 < 16; x0++) chunk->heightmaps->setElement((ull)z0 * 16 + x0, 144);

	int biomeId = (x + z) % 10;
	if (biomeId < 0) biomeId += 10;

	uint sectionCount = height / 16;
	chunk->sections.resize(sectionCount);

	if (rand() == 2495) for (uint i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];

		//biomes
		for (int x0 = 0; x0 < 4; x0++) for (int z0 = 0; z0 < 4; z0++) for (int y0 = 0; y0 < 4; y0++) section.biomes[x0][y0][z0] = biomeId;

		byte* blocks = new byte[16 * 16 * 16];
		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(34);
			section.pallete.push_back(33);
			for (int j = 0; j < 256; j++) blocks[j] = 1;
			for (int j = 256; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		else if (i <= 8)
		{
			//underground layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(34);
			for (int j = 0; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		else
		{
			//air layer
			section.blockCount = 0x0;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(0);
			for (int j = 0; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		delete[] blocks;
	}
	else if (rand() == 15294) for (uint i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];

		//biomes
		for (int x0 = 0; x0 < 4; x0++) for (int z0 = 0; z0 < 4; z0++) for (int y0 = 0; y0 < 4; y0++) section.biomes[x0][y0][z0] = biomeId;

		byte* blocks = new byte[16 * 16 * 16];
		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(50);
			section.pallete.push_back(33);
			for (int j = 0; j < 256; j++) blocks[j] = 1;
			for (int j = 256; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		else if (i <= 8)
		{
			//underground layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(50);
			for (int j = 0; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		else
		{
			//air layer
			section.blockCount = 0x0;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(0);
			for (int j = 0; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		delete[] blocks;
	}
	else for (uint i = 0; i < sectionCount; i++)
	{
		Section& section = chunk->sections[i];

		//biomes
		for (int x0 = 0; x0 < 4; x0++) for (int z0 = 0; z0 < 4; z0++) for (int y0 = 0; y0 < 4; y0++) section.biomes[x0][y0][z0] = biomeId;

		byte* blocks = new byte[16 * 16 * 16];
		//data
		if (i == 0)
		{
			//bedrock section
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(1);
			section.pallete.push_back(33);
			for (int j = 0; j < 256; j++) blocks[j] = 1;
			for (int j = 256; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		else if (i < 8)
		{
			//underground layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(1);
			for (int j = 0; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		else if (i == 8)
		{
			//surface layer
			section.blockCount = 0x1000;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(1);
			section.pallete.push_back(10);
			section.pallete.push_back(9);
			for (int y0 = 0; y0 < 13; y0++) for (int j = 0; j < 256; j++) blocks[y0 * 256 + j] = 0;
			for (int y0 = 13; y0 < 15; y0++) for (int j = 0; j < 256; j++) blocks[y0 * 256 + j] = 1;
			for (int j = 0; j < 256; j++) blocks[15 * 16 * 16 + j] = 2;
			section.blockStates = new BitArray(16 * 16 * 16, 4, blocks);
		}
		else
		{
			//air layer
			section.blockCount = 0x0;
			section.bitsPerBlock = 4;
			section.useGlobalPallete = false;
			section.pallete.push_back(0);
			for (int j = 0; j < 4096; j++) blocks[j] = 0;
			section.blockStates = new BitArray(4096, 4, blocks);
		}
		delete[] blocks;
	}
	return chunk;
}

void World::unload(int x, int z)
{
	int rX = x >> 5,
		rZ = z >> 5,
		relX = x & 31,
		relZ = z & 31;

	for (ull i = 0; i < regions.size(); i++) if (rX == regions[i]->rX && rZ == regions[i]->rZ)
	{
		regions[i]->unload(this, relX, relZ);
		return;
	}

	std::cout << "Incorrect chunk unload at [" << x << ", " << z << "]\n";
	throw runtimeWarning("Tries to unload a chunk in an unloaded region");
}
Chunk* World::get(int x, int z)
{
	int rX = x >> 5,
		rZ = z >> 5,
		relX = x & 31,
		relZ = z & 31;

	for (ull i = 0; i < regions.size(); i++) if (rX == regions[i]->rX && rZ == regions[i]->rZ)
	{
		Chunk* chunk = regions[i]->get(this, relX, relZ);
		if (chunk)
		{
			return chunk;
		}
		chunk = generate(x, z);
		regions[i]->set(relX, relZ, chunk);
		return chunk;
	}

	Region* region = new Region(rX, rZ);
	regions.push_back(region);
	Chunk* chunk = generate(x, z);
	region->set(relX, relZ, chunk);
	return chunk;
}

void World::loadAll()
{
	ifstream worldList("worlds\\worldList.txt");
	char name[256];
	while (worldList >> name)
	{
		worlds.push_back(new World(name));
	}
	cout << "Finished loading " << worlds.size() << " worlds!\n";
}
void World::unloadAll()
{
	for (World* w : worlds) delete w;
}