#pragma once
#include "nbt_base.h"

class nbt_short : public nbt
{
	bigEndian<short> value;

public:
	NBT_API nbt_short(const std::string& name = "", short value = 0);

	NBT_API void write(std::fstream&, bool includeNameAndType = true) const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true) const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	NBT_API std::string getStringValue() const;

	//get the value of the tag
	NBT_API short& vShort();
	NBT_API operator short& ();
	NBT_API void operator=(short);
};