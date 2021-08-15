#include "section.h"

Section::~Section()
{
	delete blockStates;
}