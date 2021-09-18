#include "section.h"

Section::Section() { }
Section::~Section()
{
	delete blockStates;
}

const varInt LightSection::lightArrayLength = 2048;

LightSection::~LightSection()
{
	if (skyLight) delete skyLight;
	if (blockLight) delete blockLight;
}