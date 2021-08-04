#pragma once
#include "json_base.h"
class json_string : public json
{
	std::string v;

public:
	json_string(std::string name);
	json_string(std::string name, std::string value);

	std::string stringValue();

	std::string& value();
	operator std::string& ();
	void operator=(const std::string&);
};