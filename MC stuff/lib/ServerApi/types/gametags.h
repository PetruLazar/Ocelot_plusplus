#pragma once

#include "mcString.h"
#include "varData.h"
#include "apidef.h"
#include <vector>

class TagGroup
{
public:
	//the name of the category: minecraft:blocks, minecraft:item, minecraft:fluid...
	mcString tagType;
	class Tag
	{
	public:
		mcString name;
		std::vector<varInt> entries;
	};
	std::vector<Tag> tags;

	SERVER_API static std::vector<TagGroup> defaultTags;

	SERVER_API static void loadVanillaTags();
};