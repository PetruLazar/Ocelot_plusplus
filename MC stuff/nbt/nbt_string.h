#pragma once
#include "nbt_base.h"

class nbt_string : public nbt
{
	std::string value;

public:
	nbt_string(const std::string& name = "", const std::string& value = "");

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get the value of the tag
	std::string& vString();
	operator std::string& ();
	void operator=(const std::string&);
	void operator=(const char*);
};