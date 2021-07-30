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

	//write tag to file
	void write(std::fstream&, bool includeNameAndType = true);
	//read tag from file
	void read(std::fstream&, std::string name = "");
	//write tag to buffer
	void write(char*&, bool includeNameAndType = true);
	//read tag from buffer
	void read(char*&, std::string name = "");
	//get the value of the tag as string
	std::string getStringValue();

	//get a value from an int array tag
	int& vInt(uint);
	int& operator[](uint);
	//get the size of an array tag
	uint getSize();
	//resize a list, an array or a compound tag
	virtual void resize(uint);
};