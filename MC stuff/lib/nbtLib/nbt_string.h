#pragma once
#include "nbt_base.h"

class nbt_string : public nbt
{
	std::string value;

public:
	NBT_API nbt_string(const std::string& name = "", const std::string& value = "");

	NBT_API void write(std::fstream&, bool includeNameAndType = true) const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true) const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	NBT_API std::string getStringValue() const;

	//get the value of the tag
	NBT_API std::string& vString();
	NBT_API operator std::string& ();
	NBT_API void operator=(const std::string&);
	NBT_API void operator=(const char*);
};