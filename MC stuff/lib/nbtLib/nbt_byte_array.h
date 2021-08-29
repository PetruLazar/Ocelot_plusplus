#pragma once
#include "nbt_base.h"

class nbt_byte_array : public nbt
{
	char* values;
	buint _size;

public:
	NBT_API nbt_byte_array(const std::string& name = "", char* values = nullptr, int size = 0);
	NBT_API ~nbt_byte_array();

	NBT_API void write(std::fstream&, bool includeNameAndType = true) const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true) const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	NBT_API std::string getStringValue() const;

	//get a value from the array
	NBT_API char& vByte(uint);
	NBT_API char& operator[](uint);
	//get the size of the array
	NBT_API uint getSize() const;
	//resize the array
	NBT_API void resize(uint);
};