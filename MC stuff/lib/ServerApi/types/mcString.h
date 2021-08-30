#pragma once
#include <string>
#include "apidef.h"

class mcString : public std::string
{
public:
	using std::string::basic_string;
	SERVER_API mcString();
	SERVER_API mcString(const std::string&);

	SERVER_API void read(std::fstream&);
	SERVER_API void write(std::fstream&) const;
	SERVER_API void read(char*&);
	SERVER_API void write(char*&) const;
};