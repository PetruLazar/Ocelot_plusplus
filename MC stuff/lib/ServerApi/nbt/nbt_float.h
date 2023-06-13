#pragma once
#include "nbt_base.h"

class nbt_float : public nbt
{
	bigEndian<float> value;

public:
	SERVER_API nbt_float(const std::string& name = "", float value = 0);

	SERVER_API void write(std::ostream&, bool includeNameAndType = true) const;
	SERVER_API void read(std::istream&, const std::string& name = "");
	SERVER_API void write(char*&, bool includeNameAndType = true) const;
	SERVER_API void read(char*&, const std::string& name = "");
	//get the value of the tag as a string
	SERVER_API std::string getStringValue() const;

	//get the value of the tag
	SERVER_API float& vFloat();
	SERVER_API operator float& ();
	SERVER_API void operator=(const nbt&);
	SERVER_API void operator=(const nbt_float&);
	SERVER_API void operator=(float);
};