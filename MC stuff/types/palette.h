#pragma once
#include "types\typedefs.h"
#include "types\mcString.h"

//unused - discussions needed

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

		minecraft_oak_fence,
		minecraft_oak_fence_east_true_north_true_south_true_waterlogged_true_west_true,
		minecraft_oak_fence_east_true_north_true_south_true_waterlogged_true_west_false,
		minecraft_oak_fence_east_true_north_true_south_true_waterlogged_false_west_true
		//...

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
	int from_string(const mcString& name, mcString* states)
	{
		if (name == "air") return minecraft_air;
		if (name == "stone") return minecraft_stone;
		//...
		if (name == "grass_block")
		{
			int snowy;
			for (int i = 0; i < 1; i++)
			{
				if (states[i << 1] == "snowy") snowy = i << 1;
			}
			if (states[snowy] == "true") return minecraft_grass_block_snowy_true;
			if (states[snowy] == "false") return minecraft_grass_block_snowy_false;
		}
		if (name == "oak_fence")
		{
			int east, west, north, south, waterlogged;
			for (int i = 0; i < 5; i++)
			{
				if (states[i << 1] == "east") { east = i << 1; continue; }
				if (states[i << 1] == "west") { west = i << 1; continue; }
				if (states[i << 1] == "north") { north = i << 1; continue; }
				if (states[i << 1] == "south") { south = i << 1; continue; }
				if (states[i << 1] == "waterlogged") { waterlogged = i << 1; continue; }
			}
			//test every combination of block states and return the id;
			if (states[east] == "true" && states[west] == "true" && states[south] == "true" && states[north] == "true" && states[waterlogged] == "true") return minecraft_oak_fence_east_true_north_true_south_true_waterlogged_true_west_true;
			if (states[east] == "true" && states[west] == "false" && states[south] == "true" && states[north] == "true" && states[waterlogged] == "false") return minecraft_oak_fence_east_true_north_true_south_true_waterlogged_true_west_false;
			if (states[east] == "true" && states[west] == "true" && states[south] == "true" && states[north] == "true" && states[waterlogged] == "false") return minecraft_oak_fence_east_true_north_true_south_true_waterlogged_false_west_true;
			//...
		}
		//every block existent
	}
}