#pragma once
#include "nbt_base.h"

class nbt_long_array : public nbt
{
	bigEndian<int64>* values;
	buint _size;

public:
	nbt_long_array(std::string name = "");
	nbt_long_array(int64*, int, std::string name = "");
	~nbt_long_array();

	//write tag to file
	void write(std::fstream&, bool includeNameAndTtype = true);
	//read tag from file
	void read(std::fstream&, std::string name = "");
	//write tag to buffer
	void write(char*&, bool includeNameAndType = true);
	//read tag from buffer
	void read(char*&, std::string name = "");
	//get the value of the tag as string
	std::string getStringValue();

	//get a value from a long array tag
	int64& vLong(uint);
	//get a value from a long array tag
	int64& operator[](uint);
	//get the size of an array tag
	uint getSize();
	//resize a list, an array or a compound tag
	virtual void resize(uint);
};