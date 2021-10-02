#pragma once
#include "nbt_base.h"

class nbt_int_array : public nbt
{
	bint* values;
	buint _size;

public:
	SERVER_API nbt_int_array(const std::string& name = "", int* values = nullptr, int size = 0);
	SERVER_API ~nbt_int_array();

	SERVER_API void write(std::fstream&, bool includeNameAndType = true) const;
	SERVER_API void read(std::fstream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true) const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue() const;

	//get a value from the array
	SERVER_API int& vInt(uint);
	SERVER_API int& operator[](uint);
	//get the size of the array
	SERVER_API uint getSize() const;
	//resize the array
	SERVER_API virtual void resize(uint);

	SERVER_API void operator=(const nbt&);
	SERVER_API void operator=(const nbt_int_array&);
};