#pragma once
#include "apidef.h"
#include "typedefs.h"
#include "../json.h"

#include "mcString.h"

class Registry
{
	static json* registries;
	static json* globalPalette;

public:
	SERVER_API static int getId(const std::string& category, const std::string& item);
	SERVER_API static std::string getName(const std::string& category, int id);

	SERVER_API static void loadRegistriesAndPalette();
};