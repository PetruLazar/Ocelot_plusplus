#include "json_int.h"

json_int::json_int(const std::string& name, int value) : json(integer, name), v(value) { }

int& json_int::iValue() { return v; }
json_int::operator int& () { return v; }
void json_int::operator=(int val)
{
	v = val;
}

std::string json_int::stringValue()
{
	return std::to_string(v);
}