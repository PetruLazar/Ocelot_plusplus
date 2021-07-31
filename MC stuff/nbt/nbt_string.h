#pragma once
#include "nbt_base.h"

class nbt_string : public nbt
{
	std::string value;

public:
	nbt_string(std::string name = "");
	nbt_string(std::string, std::string name = "");

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get the value of the tag
	std::string& vString();
	operator std::string& ();
};