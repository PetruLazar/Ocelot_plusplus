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

	void write(std::fstream&, bool includeNameAndTtype = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as string
	std::string getStringValue();

	//get a value from the array
	int64& vLong(uint);
	//get a value from the array
	int64& operator[](uint);
	//get the size of the array
	uint getSize();
	//resize the array
	virtual void resize(uint);
};