#pragma once
#include "nbt_base.h"

class nbt_double : public nbt
{
	bigEndian<double> value;

public:
	NBT_API nbt_double(const std::string& name = "", double value = 0);

	NBT_API void write(std::fstream&, bool includeNameAndType = true) const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true) const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	NBT_API std::string getStringValue() const;

	//get the value of the tag
	NBT_API double& vDouble();
	NBT_API operator double& ();
	NBT_API void operator=(double);
};