#pragma once

#include "block.h"
#include "item.h"
#include "fluid.h"
#include "gameevent.h"
#include "entity.h"
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

		BASICDEFS_API Tag(const mcString& name, varInt entryCount, varInt* entries);
		BASICDEFS_API ~Tag();
	}*tags;

	BASICDEFS_API Tags(varInt tagCount, const mcString& tagType, Tag* tags);
	BASICDEFS_API ~Tags();

	BASICDEFS_API static const varInt defaltTagsLengthCount;
	BASICDEFS_API static Tags* defaultTags;
};