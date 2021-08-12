#pragma once
#include "json_base.h"
class json_string : public json
{
	std::string v;

public:
	json_string(std::string name = "", std::string value = "");

	std::string stringValue() const;

	std::string& value();
	operator std::string& ();
	void operator=(const std::string&);
};