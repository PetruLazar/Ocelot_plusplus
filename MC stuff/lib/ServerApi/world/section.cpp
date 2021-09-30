#include "section.h"
#include "../types/error.h"

Section::Section() { }
Section::~Section()
{
	delete blockStates;
}

BlockState& Section::getPaletteEntry(int relX, int relY, int relZ)
{
	return palette[blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX)].block;
}
BlockState& Section::getPaletteEntry(int paletteIndex)
{
	return palette[paletteIndex].block;
}
BlockState Section::getBlock(int relX, int relY, int relZ)
{
	return palette[blockStates->getElement(((ull)relY << 8) | ((ull)relZ << 4) | relX)].block;
}
void setBlock(const BlockState& bl)
{
	throw runtimeError("Can't you just wait a few days before you try calling this function...?");
}

const varInt LightSection::lightArrayLength = 2048;

LightSection::~LightSection()
{
	if (skyLight) delete skyLight;
	if (blockLight) delete blockLight;
}