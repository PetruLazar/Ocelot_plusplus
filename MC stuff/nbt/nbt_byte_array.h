#pragma once
#include "nbt_base.h"

class nbt_byte_array : public nbt
{
	char* values;
	buint _size;

public:
	nbt_byte_array(std::string name = "");
	nbt_byte_array(char*, int, std::string name = "");
	~nbt_byte_array();

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get a value from the array
	char& vByte(uint);
	char& operator [] (uint);
	//get the size of the array
	uint getSize();
	//resize the array
	void resize(uint);
};