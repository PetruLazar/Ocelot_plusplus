#pragma once
#include "apidef.h"
#include "typedefs.h"
#include "../json.h"

#include "mcString.h"

class BlockProperty
{
public:
	const std::string name, value;

	SERVER_API BlockProperty(const std::string& name, const std::string& value);
};

class Registry
{
	static json* registries;
	static json* globalPalette;

public:
	SERVER_API static int getId(const std::string& category, const std::string& item);
	SERVER_API static std::string getName(const std::string& category, int id);

	//get the protocol id of the block, or the id of the default state if the block has multiple states
	SERVER_API static int getBlockId(const std::string& blockName);
	//get the protocol id of the block state with the desired properties, "properties" array is assumed to be the same size as the block property count, and the block is assumed to have properties
	SERVER_API static int getBlockId(const std::string& blockName, BlockProperty* properties);

	SERVER_API static void loadRegistriesAndPalette();
};