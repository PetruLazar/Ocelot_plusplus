#pragma once
#include "nbt_base.h"

class nbt_int : public nbt
{
	bint value;

public:
	nbt_int(std::string name = "");
	nbt_int(int, std::string name = "");

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get the value of the tag
	int& vInt();
	operator int& ();
};