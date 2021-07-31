#pragma once
#include "nbt_base.h"

class nbt_double : public nbt
{
	bigEndian<double> value;

public:
	nbt_double(std::string name = "");
	nbt_double(double, std::string name = "");

	void write(std::fstream&, bool includeNameAndType = true);
	void read(std::fstream&, std::string name = "");
	void write(char*&, bool includeNameAndType = true);
	void read(char*&, std::string name = "");
	//get the value of the tag as a string
	std::string getStringValue();

	//get the value of the tag
	double& vDouble();
	operator double& ();
};