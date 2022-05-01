#include "json_int.h"

json_int::json_int(const std::string& name, int value) : json(json::type::integer, name), v(value) {}

int& json_int::iValue() { return v; }
const int& json_int::iValue() const { return v; }
json_int::operator int& () { return v; }
json_int::operator const int& () const { return v; }
void json_int::operator=(int val)
{
	v = val;
}

std::string json_int::stringValue() const
{
	return std::to_string(v);
}