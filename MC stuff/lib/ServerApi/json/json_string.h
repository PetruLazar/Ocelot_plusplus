#pragma once
#include "json_base.h"

class json_string : public json
{
	std::string v;

public:
	SERVER_API json_string(std::string name = "", std::string value = "");

	SERVER_API std::string stringValue() const;

	SERVER_API std::string& value();
	SERVER_API operator std::string& ();
	void SERVER_API operator=(const std::string&);
};