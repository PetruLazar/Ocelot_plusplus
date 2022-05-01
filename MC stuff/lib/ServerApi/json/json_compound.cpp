#include "json_compound.h"
#include "../types/mcString.h"

const char notFound[] = "No element found by that name.";

json_compound::json_compound(const std::string& name, const std::vector<json*>& values) : json(json::type::compound, name), v(values) {}
json_compound::json_compound(const std::string& name, json** values, ull s) : json(json::type::compound, name)
{
	for (ull i = 0; i < s; i++) v.emplace_back(values[i]);
	delete[] values;
}
json_compound::~json_compound()
{
	for (json* e : v) delete e;
}

std::string json_compound::stringValue() const
{
	if (!v.size()) return "{}";
	std::string ret = "{";
	for (json* e : v) ret += e->to_string() + ',';
	ret.pop_back();
	return ret + '}';
}

void json_compound::write(char*& buffer) const
{
	mcString(stringValue()).write(buffer);
}
void json_compound::write(std::fstream& is) const
{
	mcString(stringValue()).write(is);
}

bool json_compound::has(const std::string& i) const
{
	for (json* e : v)
	{
		if (e->getName() == i)
			return true;
	}

	return false;
}

json& json_compound::value(int i) { return *v[i]; }
const json& json_compound::value(int i) const { return *v[i]; }
json& json_compound::operator[](int i) { return *v[i]; }
const json& json_compound::operator[](int i) const { return *v[i]; }
json& json_compound::value(const std::string& i)
{
	for (json* e : v) if (e->getName() == i) return *e;
	throw notFound;
}
const json& json_compound::value(const std::string& i) const
{
	for (json* e : v) if (e->getName() == i) return *e;
	throw notFound;
}
json& json_compound::operator[](const std::string& i)
{
	for (json* e : v) if (e->getName() == i) return *e;
	throw notFound;
}
const json& json_compound::operator[](const std::string& i) const
{
	for (json* e : v) if (e->getName() == i) return *e;
	throw notFound;
}

ull json_compound::getSize() const { return v.size(); }
void json_compound::add(json* elem) { v.emplace_back(elem); }
void json_compound::remove(json* elem)
{
	ull s = v.size();
	for (ull i = 0; i < s; i++) if (v[i] == elem)
	{
		delete v[i];
		for (ull j = i + 1; j < s; j++) v[j - 1] = v[j];
		v.pop_back();
		return;
	}
}
void json_compound::remove(const std::string& str)
{
	ull s = v.size();
	for (ull i = 0; i < s; i++) if (v[i]->getName() == str)
	{
		delete v[i];
		for (ull j = i + 1; j < s; j++) v[j - 1] = v[j];
		v.pop_back();
		return;
	}
}