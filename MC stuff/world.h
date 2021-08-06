#pragma once
#include "basic.h"
#include "nbt.h"

class World
{
	nbt_compound level_dat;

	World();
public:

	static World world;
};

