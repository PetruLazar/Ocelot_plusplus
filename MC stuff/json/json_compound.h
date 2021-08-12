#pragma once
#include "json_base.h"

class json_compound : public json
{
	std::vector<json*> v;

public:
	json_compound(const std::string& name = "", json** values = nullptr, ull size = 0);
	json_compound(const std::string& name = "", const std::vector<json*>& values = std::vector<json*>(0));
	~json_compound();

	std::string stringValue() const;
	void write(char*&) const;
	void write(std::fstream&) const;

	json& value(const std::string& i);
	json& operator[](const std::string& i);

	ull getSize();
	void add(json* elem);
	void remove(json* elem);
	void remove(const std::string&);
};

