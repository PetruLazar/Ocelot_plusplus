#include "json_string.h"

json_string::json_string(std::string name) : json_base(string, name) { }
json_string::json_string(std::string name, std::string value) : json_base(string, name), v(value) { }

std::string json_string::value()
{
	throw "WIP";
}

json_string::operator std::string()
{
	throw "WIP";
}