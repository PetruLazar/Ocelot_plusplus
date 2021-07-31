#pragma once
#include "nbt_base.h"

class nbt_int_array : public nbt
{
	bint* values;
	buint _size;

public:
	nbt_int_array(std::string name = "");
	nbt_int_array(int*, int, std::string name = "");
	~nbt_int_array();

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get a value from the array
	int& vInt(uint);
	int& operator[](uint);
	//get the size of the array
	uint getSize();
	//resize the array
	virtual void resize(uint);
};