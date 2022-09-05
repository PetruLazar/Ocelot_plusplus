#include "../world.h"
#include "../debug/mcexceptions.h"
#include "../types/utils.h"

Section::Section() {}
//Section::~Section() {}

bool Section::setBlock(int relX, int relY, int relZ, int blockid)
{
	int index = relY << 8 | relZ << 4 | relX;
	if (blockid == 0)
	{
		if (blockStates.set(index, blockid))
		{
			blockCount--;
			return true;
		}
		return false;
	}
	else
	{
		int oldBlockId = blockStates.get(index);
		if (blockStates.set(index, blockid))
		{
			if (oldBlockId == 0) blockCount++;
			return true;
		}
		return false;
	}
}

const varInt LightSection::lightArrayLength = 2048;

LightSection::~LightSection()
{
	if (skyLight) delete skyLight;
	if (blockLight) delete blockLight;
}