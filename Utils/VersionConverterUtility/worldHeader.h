#pragma once
#include "types/varData.h"
#include "regionFiles.h"

namespace Archive
{

}

void convertWorld0to1(const path& wldPath)
{
	ifstream wld(wldPath / "characteristics.bin", ios::binary);
	if (!wld.is_open())
		throw "Could not open main world file";
	varInt version;
	version.read(wld);
	if (version != 0)
		throw "Invalid version";
	if (!nbt::checkTag(wld))
		throw "World's main file has invalid format";
	nbt_compound characteristics;
	characteristics.read(wld);
	wld.close();

	convertRegions0to1(wldPath, characteristics["height"].vInt());
	
	fstream nWld(wldPath / "characteristics.bin", ios::binary | ios::in | ios::out);
	varInt(1).write(nWld); // since converting from 0 to 1, only that byte changes
	nWld.close();
}