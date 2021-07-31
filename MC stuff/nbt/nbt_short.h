#pragma once
#include "nbt_base.h"

class nbt_short : public nbt
{
	bigEndian<short> value;

public:
	nbt_short(std::string name = "");
	nbt_short(short, std::string name = "");

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get the value of the tag
	short& vShort();
	operator short& ();
};