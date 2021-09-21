#pragma once
#include "nbt_base.h"

class nbt_Byte : public nbt
{
	char value;

public:
	SERVER_API nbt_Byte(const std::string& name = "", char value = 0);

	SERVER_API void write(std::fstream&, bool includeNameAndType = true) const;
	SERVER_API void read(std::fstream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true) const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue() const;

	//get the value of the tag
	SERVER_API char& vByte();
	SERVER_API operator char& ();
	SERVER_API void operator=(char);
};