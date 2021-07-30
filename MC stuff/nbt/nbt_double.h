#pragma once
#include "nbt_base.h"

class nbt_double : public nbt
{
	bigEndian<double> value;

public:
	nbt_double(std::string name = "");
	nbt_double(double, std::string name = "");
	//~nbt_double();

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

	//get the value of a double tag
	double& vDouble();
	operator double& ();
};