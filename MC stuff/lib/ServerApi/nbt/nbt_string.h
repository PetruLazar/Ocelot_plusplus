#pragma once
#include "nbt_base.h"

class nbt_string : public nbt
{
	std::string value;

public:
	SERVER_API nbt_string(const std::string& name = "", const std::string& value = "");

	SERVER_API void write(std::ostream&, bool includeNameAndType = true) const;
	SERVER_API void read(std::istream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true) const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue() const;

	//get the value of the tag
	SERVER_API std::string& vString();
	SERVER_API operator std::string& ();
	SERVER_API void operator=(const nbt&);
	SERVER_API void operator=(const nbt_string&);
	SERVER_API void operator=(const std::string&);
	SERVER_API void operator=(const char*);
};