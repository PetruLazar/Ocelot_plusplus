#include "json_base.h"
#include "../json.h"

const char typeError[] = "Invalid json element type.";

json::json(type t, const std::string& name) : t(t), name(name) { }
json::~json() { }

std::string json::getName() { return name; }
json::type json::getType() { return t; }
json::type json::getChildType() { throw typeError; };
void json::changeType(type) { throw typeError; }

std::string json::to_string() const
{
	return '"' + name + "\":" + stringValue();
}

int& json::iValue() { throw typeError; }
bool& json::bValue() { throw typeError; }
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
	case boolean:
		return new json_boolean();
	}
	throw typeError;
}

//returns the next char that is not space or endl
int skipInJsonFile(std::fstream& f)
{
	int ch = f.get();
	while (ch == ' ' || ch == '\n' || ch == '\t')
	{
		ch = f.get();
	}
	return ch;
}

const char invalidJson[] = "Invalid json file";

json* json::parse(std::fstream& f, bool canHaveName)
{
	int ch = skipInJsonFile(f);
	std::string name;
	if (ch == '"' && canHaveName)
	{
		//name (optional)
		while (true)
		{
			ch = f.get();
			if (ch == '"') break;
			name.push_back(ch);
		}
		ch = skipInJsonFile(f);
		if (ch != ':')
		{
			throw runtimeError(invalidJson);
		}
		ch = skipInJsonFile(f);
	}
	if (ch == '{')
	{
		//compound
		json* ret = new json_compound(name, nullptr);
		do
		{
			ret->add(parse(f));
			ch = skipInJsonFile(f);
		} while (ch == ',');

		if (ch != '}') throw runtimeError(invalidJson);
		return ret;
	}
	else if (ch == '[')
	{
		//array
		std::vector<json*> elems;
		do
		{
			elems.push_back(parse(f, false));
			ch = skipInJsonFile(f);
		} while (ch == ',');

		if (ch != ']') throw runtimeError(invalidJson);

		//make a traditional array with the values in elems and construct the array json
		ull size = elems.size();
		json** jsons = new json * [size];
		for (int i = 0; i < size; i++) jsons[i] = elems[i];
		return new json_array(name, jsons, size);
	}
	else if (ch == '"')
	{
		//string or bool
		std::string value;
		while (true)
		{
			ch = f.get();
			if (ch == '"') break;
			value.push_back(ch);
		}
		if (value == "false")
		{
			//bool - false
			return new json_boolean(name, false);
		}
		if (value == "true")
		{
			//bool - true
			return new json_boolean(name, true);
		}
		return new json_string(name, value);
	}
	else if (ch >= '0' && ch <= '9')
	{
		//int
		int value = ch - '0';
		while (true)
		{
			ch = f.get();
			if (ch < '0' || ch>'9') break;
			value = value * 10 + ch - '0';
		}
		f.unget();
		return new json_int(name, value);
	}
	else if (ch == 'f' || ch == 't')
	{
		std::string check;
		check.push_back(ch);
		if (ch == 'f')
		{
			for (int i = 0; i < 4; i++) check.push_back(f.get());
			if (check != "false")
			{
				throw runtimeError(invalidJson);
			}
			return new json_boolean(name, false);
		}
		else
		{
			for (int i = 0; i < 3; i++) check.push_back(f.get());
			if (check != "true")
			{
				throw runtimeError(invalidJson);
			}
			return new json_boolean(name, true);
		}
	}
	else
	{
		throw runtimeError(invalidJson);
	}
	return nullptr;
}