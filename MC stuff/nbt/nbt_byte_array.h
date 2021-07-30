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

	//write tag to file
	void write(std::fstream&, bool includeNameAndType = true);
	//read tag from tile
	void read(std::fstream&, std::string name = "");
	//write tag to buffer
	void write(char*&, bool includeNameAndType = true);
	//read tag from buffer
	void read(char*&, std::string name = "");
	//get the value of the tag as string
	std::string getStringValue();

	//get a value of a byte array tag
	char& vByte(uint);
	char& operator [] (uint);
	//get the size of an array tag
	uint getSize();
	//resize a list, an array or a compound tag
	void resize(uint);
};