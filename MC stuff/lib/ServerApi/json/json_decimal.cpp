#include "json_decimal.h"

json_decimal::json_decimal(const std::string& name, double value) : json(json::type::decimal, name), v(value) { }

double& json_decimal::dValue() { return v; }
const double& json_decimal::dValue() const { return v; }
json_decimal::operator double& () { return v; }
json_decimal::operator const double& () const { return v; }

void json_decimal::operator=(double val)
{
	v = val;
}

std::string json_decimal::stringValue() const
{
	return std::to_string(v);
}