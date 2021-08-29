#pragma once
#include "json_base.h"

class json_string : public json
{
	std::string v;

public:
	JSON_API json_string(std::string name = "", std::string value = "");

	JSON_API std::string stringValue() const;

	JSON_API std::string& value();
	JSON_API operator std::string& ();
	void JSON_API operator=(const std::string&);
};