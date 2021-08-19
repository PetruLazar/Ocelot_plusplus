#pragma once

#include "block.h"
#include "item.h"
#include "fluid.h"
#include "gameevent.h"
#include "entity.h"
#include "typedefs.h"
#include "mcString.h"
#include "varData.h"

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

		Tag(const mcString& name, varInt entryCount, varInt* entries);
		~Tag();
	}*tags;

	Tags(varInt tagCount, const mcString& tagType, Tag* tags);
	~Tags();

	static const varInt defaltTagsLengthCount;
	static Tags* defaultTags;
};