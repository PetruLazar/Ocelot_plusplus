#pragma once
#include "basic.h"

class mcString : public std::string
{
public:
	using std::string::basic_string;
	mcString();
	mcString(const std::string&);

	void read(std::fstream&);
	void write(std::fstream&) const;
	void read(char*&);
	void write(char*&) const;
};

