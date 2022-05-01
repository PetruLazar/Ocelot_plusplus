#pragma once
#include "json_base.h"
#include <vector>

class json_compound : public json
{
	std::vector<json*> v;

public:
	SERVER_API json_compound(const std::string& name = "", json** values = nullptr, ull size = 0);
	SERVER_API json_compound(const std::string& name, const std::vector<json*>& values = std::vector<json*>(0));
	SERVER_API ~json_compound();

	std::string SERVER_API stringValue() const;
	void SERVER_API write(char*&) const;
	void SERVER_API write(std::fstream&) const;

	bool has(const std::string& i) const;

	SERVER_API json& value(int i);
	SERVER_API const json& value(int i) const;
	SERVER_API json& operator[](int i);
	SERVER_API const json& operator[](int i) const;
	SERVER_API json& value(const std::string& i);
	SERVER_API const json& value(const std::string& i) const;
	SERVER_API json& operator[](const std::string& i);
	SERVER_API const json& operator[](const std::string& i) const;

	ull SERVER_API getSize() const;
	void SERVER_API add(json* elem);
	void SERVER_API remove(json* elem);
	void SERVER_API remove(const std::string&);
};

