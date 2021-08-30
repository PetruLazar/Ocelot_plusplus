#pragma once
#include "nbt_base.h"
#include "../types/bitarray.h"

class nbt_long_array : public nbt
{
	blong* values;
	buint _size;

public:
	SERVER_API nbt_long_array(const std::string& name = "", int64* values = nullptr, int size = 0);
	SERVER_API nbt_long_array(const std::string& name, const BitArray& bitarray);
	SERVER_API ~nbt_long_array();

	SERVER_API void write(std::fstream&, bool includeNameAndTtype = true) const;
	SERVER_API void read(std::fstream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true) const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as string
	SERVER_API std::string getStringValue() const;

	//get a value from the array
	SERVER_API int64& vLong(uint);
	//get a value from the array
	SERVER_API int64& operator[](uint);
	//get the size of the array
	SERVER_API uint getSize() const;
	//resize the array
	SERVER_API virtual void resize(uint);
};