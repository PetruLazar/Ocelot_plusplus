#pragma once
#include "typedefs.h"
#include "mcString.h"

namespace globalPalette
{
	//just for reference
	enum blocks
	{
		minecraft_air = 0,
		minecraft_stone = 1,
		minecraft_granite = 2,
		minecraft_polished_granite = 3,

		minecraft_grass_block_snowy_true = 4, //snowy=true
		minecraft_grass_block_snowy_false = 5, //snowy=false, even tho it is default
		minecraft_grass_block = 5, //default

		minecraft_oak_fence
		//...
	};

	//vector with the block and it's states
	mcString* to_string(int id, int& propertyCount)
	{
		switch (id)
		{
		case minecraft_air:
			propertyCount = 0;
			return new mcString[1]{ "air" };
		case minecraft_stone:
			propertyCount = 0;
			return new mcString[1]{ "stone" };
			//...
		case minecraft_grass_block_snowy_true:
			propertyCount = 1;
			return new mcString[2 * 1 + 1]{ "grass_block","snowy","true" };
		case minecraft_grass_block_snowy_false:
			propertyCount = 1;
			return new mcString[2]{ "grass_block","false" };
			//...
		}
	}
	int from_string(mcString* blockAndStates)
	{
		if (blockAndStates[0] == "air") return minecraft_air;
		if (blockAndStates[0] == "stone") return minecraft_stone;
		//...
		if (blockAndStates[0] == "grass_block")
		{
			int snowy = 0;
			if (blockAndStates[snowy] == "true") return minecraft_grass_block_snowy_true;
			if (blockAndStates[snowy] == "false") return minecraft_grass_block_snowy_false;
		}
	}
}