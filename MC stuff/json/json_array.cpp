#include "json_array.h"

const char invalidArrayElements[] = "Not all elements of the array are of the same type.";

json_array::json_array(const std::string& name) : json(array, name), s(0), v(0), childType(none) { }
json_array::json_array(const std::string& name, json** values, ull size) : json(array, name), s(size), v(values)
{
	if (!s)
	{
		v = nullptr;
		return;
	}
	childType = v[0]->getType();
	for (ull i = 1; i < s; i++) if (v[i]->getType() != childType) throw invalidArrayElements;
}
json_array::~json_array()
{
	//only assign manually allocated object when constructing the object
	for (ull i = 0; i < s; i++) if (v[i]) delete v[i];
	if (v) delete[] v;
}

std::string json_array::stringValue()
{
	if (!s) return "[]";
	std::string ret = "[";
	for (ull i = 0; i < s; i++) ret += v[i]->stringValue() + ',';
	ret.pop_back();
	return ret + ']';
}

json& json_array::value(int i) { return *v[i]; }
json& json_array::operator[](int i) { return *v[i]; }

json::type json_array::getChildType() { return childType; }
void json_array::changeType(type newType)
{
	if (childType == newType) return;
	childType = newType;
	for (ull i = 0; i < s; i++)
	{
		if (v[i]) delete v[i];
		v[i] = allocate(childType);
	}
}
ull json_array::getSize() { return s; }
void json_array::resize(ull ns)
{
	json** nv = new json * [ns];

	ull mins = s < ns ? s : ns;
	for (ull i = 0; i < mins; i++) nv[i] = v[i];
	for (ull i = mins; i < s; i++) if (v[i]) delete v[i];
	for (ull i = mins; i < ns; i++) nv[i] = allocate(childType);
}