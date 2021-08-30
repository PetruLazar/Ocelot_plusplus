#include "json_boolean.h"

json_boolean::json_boolean(const std::string& name, bool value) : json(boolean, name), v(value) { }

bool& json_boolean::bValue() { return v; }
json_boolean::operator bool& () { return v; }

void json_boolean::operator=(int val) {
	v = val;
}

void json_boolean::operator=(bool val) {
	v = val;
}

std::string json_boolean::stringValue() const
{
	if (v)
		return "true";

	return "false";
}