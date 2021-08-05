#include "json_base.h"
#include "json.h"

const char typeError[] = "Invalid json element type.";

json::json(type t, const std::string& name) : t(t), name(name) { }
json::~json() { }

std::string json::getName() { return name; }
json::type json::getType() { return t; }
json::type json::getChildType() { throw typeError; };
void json::changeType(type) { throw typeError; }

std::string json::to_string()
{
	return '"' + name + "\":" + stringValue();
}

int& json::iValue() { throw typeError; }
std::string& json::value() { throw typeError; }
json& json::value(int) { throw typeError; }
json& json::value(const std::string&) { throw typeError; }
void json::operator=(int) { throw typeError; }
void json::operator=(const std::string&) { throw typeError; }
json& json::operator[](int) { throw typeError; }
json& json::operator[](const std::string&) { throw typeError; }

ull json::getSize() { throw typeError; }
void json::resize(ull) { throw typeError; }
void json::add(json*) { throw typeError; }
void json::remove(json*) { throw typeError; }
void json::remove(const std::string&) { throw typeError; }

json* json::allocate(type tp)
{
	switch (tp)
	{
	case none:
		return nullptr;
	case integer:
		return new json_int();
	case string:
		return new json_string();
	case array:
		return new json_array();
	case compound:
		return new json_compound("", nullptr);
	}
	throw typeError;
}