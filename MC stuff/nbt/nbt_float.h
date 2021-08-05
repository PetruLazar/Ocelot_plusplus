#pragma once
#include "nbt_base.h"

class nbt_float : public nbt
{
	bigEndian<float> value;

public:
	nbt_float(const std::string& name = "");
	nbt_float(float, const std::string& name = "");

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get the value of the tag
	float& vFloat();
	operator float& ();
	void operator=(float);
};