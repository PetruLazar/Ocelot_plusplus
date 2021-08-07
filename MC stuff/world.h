#pragma once
#include "basic.h"
#include "nbt.h"
#include "options.h"
#include "region.h"

class World
{
	static nbt_compound level_dat;

	static std::vector<region*> regions;

public:
	static nbt_compound dimension_codec;
	static nbt_compound dimension;
	static nbt_compound heightMap;

	static void load();
	static void load(int, int);
	static void save();
	static void save(int, int);

	static void createNewLevelDat();
};

