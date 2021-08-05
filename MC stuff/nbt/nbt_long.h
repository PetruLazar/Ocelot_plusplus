#pragma once
#include "nbt_base.h"

class nbt_long : public nbt
{
	bigEndian<int64> value;

public:
	nbt_long(const std::string& name = "");
	nbt_long(int64, const std::string& name = "");
	
	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get the value of the tag
	int64& vLong();
	operator int64& ();
	void operator=(int64);
};