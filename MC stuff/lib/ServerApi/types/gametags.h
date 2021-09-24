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

	//get a pointer to a loaded tag (nullptr if the tag is not loaded)
	SERVER_API const Tag* getTag(const std::string& category, const std::string& tag);
	SERVER_API void loadTag(const std::string& tag);

	SERVER_API static void loadVanillaTags();
};