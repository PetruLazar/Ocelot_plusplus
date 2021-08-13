#include "world.h"

using namespace std;

const char invalidMainFile[] = "Invalid characteristics.bin file.";
const char noAccesMainFile[] = "Could not open characteristics.bin file";
const ull levelDatMaxSize = 1024 * 1024; //size after decompression

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
	fstream worldMain("worlds\\" + name + "\\characteristics.bin", ios::binary | ios::in);
	if (!worldMain.is_open())
	{
		cout << "World \"" << c_name << "\": cannot open charactestics.bin\n";
		throw 0;
	}
	if (!nbt::checkTag(worldMain))
	{
		cout << "World \"" << c_name << "\": charactestics.bin has an invalid format\n";
		throw 0;
	}
	characteristics.read(worldMain);
}

Chunk* World::generate(int x, int z)
{
	//predefined chunks for now
	Chunk* chunk = new Chunk;

	//heightmap generation
	uint bitsPerHeight = bitCount((int&)characteristics["height"]);
	ull heightEntries = 256 / (64 / bitsPerHeight);
	chunk->heightmaps.add(new nbt_long_array("MOTION_BLOCKING", new int64[48]{}, 48));
}

Chunk* World::get(int x, int z)
{
	int rX = x >> 5,
		rZ = z >> 5,
		relX = x & 31,
		relZ = z & 31;

	for (ull i = 0; i < regions.size(); i++) if (rX == regions[i]->rX && rZ == regions[i]->rZ)
	{
		Chunk* chunk = regions[i]->get(relX, relZ);
		if (!chunk)
		{
			chunk = generate(x, z);
			regions[i]->set(relX, relZ, chunk);
		}
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
}