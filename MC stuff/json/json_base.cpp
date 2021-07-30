#include "json_base.h"

json_base::json_base(type t) : t(t) { }
json_base::json_base(type t, std::string name) : t(t), name(name) { }

std::string json_base::getName()
{
	return name;
}
json_base::type json_base::getType()
{
	return t;
}

int json_base::iValue()
{
	throw 0;
}
std::string json_base::value()
{
	throw 0;
}
json_base* json_base::value(int)
{
	throw 0;
}
json_base* json_base::value(std::string)
{
	throw 0;
}