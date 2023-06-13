#pragma once
#include "nbt_base.h"

class nbt_Byte_array : public nbt
{
	char* values;
	buint _size;

public:
	SERVER_API nbt_Byte_array(const std::string& name = "", char* values = nullptr, int size = 0);
	SERVER_API ~nbt_Byte_array();

	SERVER_API void write(std::ostream&, bool includeNameAndType = true) const;
	SERVER_API void read(std::istream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true) const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue() const;

	//get a value from the array
	SERVER_API char& vByte(uint);
	SERVER_API char& operator[](uint);
	//get the size of the array
	SERVER_API uint getSize() const;
	//resize the array
	SERVER_API void resize(uint);

	SERVER_API void operator=(const nbt&);
	SERVER_API void operator=(const nbt_Byte_array&);
};