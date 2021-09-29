#pragma once
#include "../types/apidef.h"
#include "../types/varData.h"
#include "../json.h"
#include "../types/registry.h"

class PaletteEntry
{
public:
	short referenceCount;
	class Block
	{
		json* state;

	public:
		varInt id;

		SERVER_API std::string getState(const std::string&);
		SERVER_API void setState(const std::string& state, const std::string& value);

		SERVER_API void set(int blockId);
		SERVER_API void set(const std::string& blockName);
		SERVER_API void set(const std::string& blockName, BlockProperty* properties);
	} block;
};