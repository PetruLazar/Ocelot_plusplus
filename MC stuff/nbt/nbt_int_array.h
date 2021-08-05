#pragma once
#include "nbt_base.h"

class nbt_int_array : public nbt
{
	bint* values;
	buint _size;

public:
	nbt_int_array(const std::string& name = "");
	nbt_int_array(int*, int, const std::string& name = "");
	~nbt_int_array();

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get a value from the array
	int& vInt(uint);
	int& operator[](uint);
	//get the size of the array
	uint getSize() const;
	//resize the array
	virtual void resize(uint);
};