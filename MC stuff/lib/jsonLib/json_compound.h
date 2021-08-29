#pragma once
#include "json_base.h"
#include <vector>

class json_compound : public json
{
	std::vector<json*> v;

public:
	JSON_API json_compound(const std::string& name = "", json** values = nullptr, ull size = 0);
	JSON_API json_compound(const std::string& name, const std::vector<json*>& values = std::vector<json*>(0));
	JSON_API ~json_compound();

	std::string JSON_API stringValue() const;
	void JSON_API write(char*&) const;
	void JSON_API write(std::fstream&) const;

	JSON_API json& value(int i);
	JSON_API json& operator[](int i);
	JSON_API json& value(const std::string& i);
	JSON_API json& operator[](const std::string& i);

	ull JSON_API getSize();
	void JSON_API add(json* elem);
	void JSON_API remove(json* elem);
	void JSON_API remove(const std::string&);
};

