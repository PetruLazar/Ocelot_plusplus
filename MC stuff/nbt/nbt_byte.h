#pragma once
#include "nbt_base.h"

class nbt_byte : public nbt
{
	char value;

public:
	nbt_byte(const std::string& name = "", char value = 0);

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get the value of the tag
	char& vByte();
	operator char& ();
	void operator=(char);
};