#pragma once
#include "nbt_base.h"

class nbt_int : public nbt
{
	bint value;

public:
	nbt_int(const std::string& name = "", int value = 0);

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get the value of the tag
	int& vInt();
	operator int& ();
	void operator=(int);
};