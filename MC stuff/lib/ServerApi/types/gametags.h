#pragma once

#include "mcString.h"
#include "varData.h"
#include "apidef.h"

class Tags
{
public:
	varInt tagCount;
	//the name of the category: minecraft:blocks, minecraft:item, minecraft:fluid...
	mcString tagType;
	class Tag
	{
	public:
		mcString name;
		varInt entryCount;
		varInt* entries;

		SERVER_API Tag(const mcString& name, varInt entryCount, varInt* entries);
		SERVER_API ~Tag();
	}*tags;

	SERVER_API Tags(varInt tagCount, const mcString& tagType, Tag* tags);
	SERVER_API ~Tags();

	SERVER_API static const varInt defaltTagsLengthCount;
	SERVER_API static Tags* defaultTags;
};