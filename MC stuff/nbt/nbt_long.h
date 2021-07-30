#pragma once
#include "nbt_base.h"

class nbt_long : public nbt
{
	bigEndian<int64> value;

public:
	nbt_long(std::string name = "");
	nbt_long(int64, std::string name = "");
	//~nbt_long();

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

	//get the value of a long tag
	int64& vLong();
	operator int64& ();
};