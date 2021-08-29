#pragma once
#include "nbt_base.h"
#include "../BasicDefs/bitarray.h"

class nbt_long_array : public nbt
{
	blong* values;
	buint _size;

public:
	NBT_API nbt_long_array(const std::string& name = "", int64* values = nullptr, int size = 0);
	NBT_API nbt_long_array(const std::string& name, const BitArray& bitarray);
	NBT_API ~nbt_long_array();

	NBT_API void write(std::fstream&, bool includeNameAndTtype = true) const;
	NBT_API void read(std::fstream&, const std::string& name = "");
	NBT_API void write(char*&, bool includeNameAndType = true) const;
	NBT_API void read(char*&, const std::string& name = "");
	//get the value of the tag as string
	NBT_API std::string getStringValue() const;

	//get a value from the array
	NBT_API int64& vLong(uint);
	//get a value from the array
	NBT_API int64& operator[](uint);
	//get the size of the array
	NBT_API uint getSize() const;
	//resize the array
	NBT_API virtual void resize(uint);
};