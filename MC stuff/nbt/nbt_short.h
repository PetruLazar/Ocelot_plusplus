#pragma once
#include "nbt_base.h"

class nbt_short : public nbt
{
	bigEndian<short> value;

public:
	nbt_short(std::string name = "");
	nbt_short(short, std::string name = "");
	//~nbt_short();

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

	//get the value of a short tag
	short& vShort();
	operator short& ();
};