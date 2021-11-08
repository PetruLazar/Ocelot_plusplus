#pragma once
#include "../types/apidef.h"
#include "../types/varData.h"
#include "../json.h"
#include "../types/registry.h"

class BlockState
{
	json* state;

public:
	varInt id;

	SERVER_API BlockState(int id);
	SERVER_API BlockState(const std::string blockName);
	SERVER_API BlockState(const std::string blockName, BlockProperty* properties);
	SERVER_API BlockState(json* blockState);
	SERVER_API BlockState(json& blockState);

	SERVER_API std::string getState(const std::string&) const;
	SERVER_API json* getJsonState() const;
	SERVER_API void setState(const std::string& state, const std::string& value);

	SERVER_API void set(int blockId);
	SERVER_API void set(const std::string& blockName);
	SERVER_API void set(const std::string& blockName, BlockProperty* properties);

	SERVER_API bool operator==(const BlockState&) const;
	SERVER_API void operator=(const BlockState&);
};

class PaletteEntry
{
public:
	short referenceCount;
	BlockState block;

	SERVER_API PaletteEntry(const BlockState&, short refCount);
};