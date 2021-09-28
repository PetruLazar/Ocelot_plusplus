#pragma once
#include "../types/apidef.h"
#include "../types/varData.h"
#include "../json.h"

class PaletteEntry
{
public:
	short referenceCount;
	class Block
	{
		json* state;
	public:
		varInt id;

		std::string getState(const std::string&);
	} block;
};