#pragma once
#include "nbt_base.h"

class nbt_float : public nbt
{
	bigEndian<float> value;

public:
	nbt_float(std::string name = "");
	nbt_float(float, std::string name = "");

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get the value of the tag
	float& vFloat();
	operator float& ();
};