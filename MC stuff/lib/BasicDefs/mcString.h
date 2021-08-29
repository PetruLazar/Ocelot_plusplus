#pragma once
#include <string>
#include "apidef.h"

class mcString : public std::string
{
public:
	using std::string::basic_string;
	BASICDEFS_API mcString();
	BASICDEFS_API mcString(const std::string&);

	BASICDEFS_API void read(std::fstream&);
	BASICDEFS_API void write(std::fstream&) const;
	BASICDEFS_API void read(char*&);
	BASICDEFS_API void write(char*&) const;
};