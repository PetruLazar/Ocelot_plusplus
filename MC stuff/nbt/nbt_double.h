#pragma once
#include "nbt_base.h"

class nbt_double : public nbt
{
	bigEndian<double> value;

public:
	nbt_double(const std::string& name = "");
	nbt_double(double, const std::string& name = "");

	void write(std::fstream&, bool includeNameAndType = true) const;
	void read(std::fstream&, const std::string& name = "");
	void write(char*&, bool includeNameAndType = true) const;
	void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	std::string getStringValue() const;

	//get the value of the tag
	double& vDouble();
	operator double& ();
	void operator=(double);
};