#pragma once
#include "json_base.h"

class json_compound : public json
{
	std::vector<json*> v;

public:
	json_compound(const std::string& name);
	json_compound(const std::string& name, json** values, ull size);
	json_compound(const std::string& name, const std::vector<json*>& values);
	~json_compound();

	std::string stringValue();

	json& value(const std::string& i);
	json& operator[](const std::string& i);

	ull getSize();
	void add(json* elem);
	void remove(json* elem);
	void remove(const std::string&);
};

