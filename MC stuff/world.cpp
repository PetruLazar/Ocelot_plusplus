#include "world.h"

using namespace std;

const char invalidLevelDatFile[] = "The level.dat file is invalid.";
const ull levelDatMaxSize = 1024 * 1024; //size after decompression

nbt_compound World::level_dat("", nullptr);
nbt_compound World::dimension_codec("", new nbt* [1]{
	new nbt_compound("minecraft:dimension_type",new nbt * [2]{
		new nbt_string("type","minecraft:dimension_type"),
		new nbt_list("value",new nbt * [1]{
			new nbt_compound("",new nbt * [3]{
				new nbt_string("name","minecraft:overworld"),
				new nbt_int("id",0),
				new nbt_compound("element",new nbt * [15]{
					new nbt_byte("piglin_safe",1),
					new nbt_byte("natural",1),
					new nbt_float("ambient_light",1.f),
					new nbt_string("infiniburn",""),
					new nbt_byte("respawn_anchor_works",0),
					new nbt_byte("has_skylight",1),
					new nbt_byte("bed_works",1),
					new nbt_string("effects","minecraft:overworld"),
					new nbt_byte("has_raids",0),
					new nbt_int("min_y",0),
					new nbt_int("height",256),
					new nbt_int("logical_height",256),
					new nbt_float("coordinate_scale",1.f),
					new nbt_byte("ultrawarm",0),
					new nbt_byte("has_ceiling",0)
				},15)
			},3)
		},1)
	}, 2),
	//new nbt_compound("minecraft:worldgen/biome",new nbt * [2]{
	//	new nbt_string("type","minecraft:worldgen/biome")
	//},2)
	}, 1);
nbt_compound World::dimension("", new nbt* [15]{
						new nbt_byte("piglin_safe",1),
						new nbt_byte("natural",1),
						new nbt_float("ambient_light",1.f),
						new nbt_string("infiniburn",""),
						new nbt_byte("respawn_anchor_works",0),
						new nbt_byte("has_skylight",1),
						new nbt_byte("bed_works",1),
						new nbt_string("effects","minecraft:overworld"),
						new nbt_byte("has_raids",0),
						new nbt_int("min_y",0),
						new nbt_int("height",256),
						new nbt_int("logical_height",256),
						new nbt_float("coordinate_scale",1.f),
						new nbt_byte("ultrawarm",0),
						new nbt_byte("has_ceiling",0)
	}, 15);
nbt_compound World::heightMap("", new nbt* [1]{
	new nbt_long_array("MOTION_BLOCKING",new int64[36]{0x1},36)
	}, 1);

void World::load()
{
	ifstream level_dat_file(Options::level_name() + "\\level.dat", ios::binary | ios::ate);
	if (!level_dat_file.is_open())
	{
		cout << "\"level.dat\" file not found, generating new one.";
		createNewLevelDat();
		level_dat_file.open(Options::level_name() + "\\level.dat", ios::binary | ios::ate);
	}
	ull compressedSize = level_dat_file.tellg();
	uint size;
	level_dat_file.seekg(0);

	char* compressedData = new char[compressedSize],
		* data = new char[levelDatMaxSize], * buffer = data;
	level_dat_file.read(compressedData, compressedSize);

	gzipDecompress(compressedData, (uint)compressedSize, buffer, levelDatMaxSize, size);

	delete[] compressedData;
	if (!nbt::checkTag(buffer))
	{
		delete[] data;
		throw invalidLevelDatFile;
	}
	level_dat.read(buffer);

	delete[] data;
}

void World::createNewLevelDat()
{
	throw "Ma-ta";
}