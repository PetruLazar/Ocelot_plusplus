#include "json_boolean.h"

json_boolean::json_boolean(const std::string& name, bool value) : json(json::type::boolean, name), v(value) { }

bool& json_boolean::bValue() { return v; }
const bool& json_boolean::bValue() const { return v; }
json_boolean::operator bool& () { return v; }
json_boolean::operator const bool& () const { return v; }

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