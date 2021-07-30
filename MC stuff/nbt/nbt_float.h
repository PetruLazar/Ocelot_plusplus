#pragma once
#include "nbt_base.h"

class nbt_float : public nbt
{
	bigEndian<float> value;

public:
	nbt_float(std::string name = "");
	nbt_float(float, std::string name = "");
	//~nbt_float();

	//write tag to file
	void write(std::fstream&, bool includeNameAndType = true);
	//read tag from file
	void read(std::fstream&, std::string name = "");
	//write tag to buffer
	void write(char*&, bool includeNameAndType = true);
	//read tag from buffer
	void read(char*&, std::string name = "");
	//get the value of the tag as string
	std::string getStringValue();

	//get the value of a float tag
	float& vFloat();
	operator float& ();
};