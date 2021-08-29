#pragma once
#include "nbt_base.h"

class nbt_int_array : public nbt
{
	bint* values;
	buint _size;

public:
	NBT_API nbt_int_array(const std::string& name = "", int* values = nullptr, int size = 0);
	NBT_API ~nbt_int_array();

	NBT_API void write(std::fstream&, bool includeNameAndType = true) const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true) const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	NBT_API std::string getStringValue() const;

	//get a value from the array
	NBT_API int& vInt(uint);
	NBT_API int& operator[](uint);
	//get the size of the array
	NBT_API uint getSize() const;
	//resize the array
	NBT_API virtual void resize(uint);
};