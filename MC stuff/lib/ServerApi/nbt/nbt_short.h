#pragma once
#include "nbt_base.h"

class nbt_short : public nbt
{
	bigEndian<short> value;

public:
	SERVER_API nbt_short(const std::string& name = "", short value = 0);

	SERVER_API void write(std::fstream&, bool includeNameAndType = true) const;
	SERVER_API void read(std::fstream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true) const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue() const;

	//get the value of the tag
	SERVER_API short& vShort();
	SERVER_API operator short& ();
	SERVER_API void operator=(const nbt&);
	SERVER_API void operator=(const nbt_short&);
	SERVER_API void operator=(short);
};