#pragma once
#include "nbt_base.h"

class nbt_long : public nbt
{
	bigEndian<int64> value;

public:
	nbt_long(std::string name = "");
	nbt_long(int64, std::string name = "");
	
	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get the value of the tag
	int64& vLong();
	operator int64& ();
};