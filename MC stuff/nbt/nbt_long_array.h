#pragma once
#include "nbt_base.h"
#include "../types/bitarray.h"

class nbt_long_array : public nbt
{
	blong* values;
	buint _size;

public:
	nbt_long_array(const std::string& name = "", int64* values = nullptr, int size = 0);
	nbt_long_array(const std::string& name, const BitArray& bitarray);
	~nbt_long_array();

	void write(std::fstream&, bool includeNameAndTtype = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as string
	std::string getStringValue() const;

	//get a value from the array
	int64& vLong(uint);
	//get a value from the array
	int64& operator[](uint);
	//get the size of the array
	uint getSize() const;
	//resize the array
	virtual void resize(uint);
};