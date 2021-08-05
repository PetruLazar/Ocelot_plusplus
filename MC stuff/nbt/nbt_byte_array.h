#pragma once
#include "nbt_base.h"

class nbt_byte_array : public nbt
{
	char* values;
	buint _size;

public:
	nbt_byte_array(const std::string& name = "", char* values = nullptr, int size = 0);
	~nbt_byte_array();

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get a value from the array
	char& vByte(uint);
	char& operator[](uint);
	//get the size of the array
	uint getSize() const;
	//resize the array
	void resize(uint);
};