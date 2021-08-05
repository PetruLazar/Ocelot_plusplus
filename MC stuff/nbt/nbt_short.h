#pragma once
#include "nbt_base.h"

class nbt_short : public nbt
{
	bigEndian<short> value;

public:
	nbt_short(const std::string& name = "", short value = 0);

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get the value of the tag
	short& vShort();
	operator short& ();
	void operator=(short);
};