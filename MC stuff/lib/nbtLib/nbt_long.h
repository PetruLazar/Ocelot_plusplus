#pragma once
#include "nbt_base.h"

class nbt_long : public nbt
{
	bigEndian<int64> value;

public:
	NBT_API nbt_long(const std::string& name = "", int64 value = 0);

	NBT_API void write(std::fstream&, bool includeNameAndType = true) const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true) const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	NBT_API std::string getStringValue() const;

	//get the value of the tag
	NBT_API int64& vLong();
	NBT_API operator int64& ();
	NBT_API void operator=(int64);
};