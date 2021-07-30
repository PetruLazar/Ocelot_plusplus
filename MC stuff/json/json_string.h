#pragma once
#include "json_base.h"
class json_string : private json_base
{
	std::string v;
public:

	json_string(std::string name);
	json_string(std::string name, std::string value);

	std::string value();

	operator std::string();
};

