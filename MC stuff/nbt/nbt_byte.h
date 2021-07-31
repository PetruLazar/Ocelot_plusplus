#pragma once
#include "nbt_base.h"

class nbt_byte : public nbt
{
	char value;

public:
	nbt_byte(std::string name = "");
	nbt_byte(char, std::string name = "");

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get the value of the tag
	char& vByte();
	operator char& ();
};