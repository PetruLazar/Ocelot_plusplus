#pragma once
#include "json_base.h"
class json_array : public json
{
	ull s;
	json** v;
	type childType;

public:
	json_array(const std::string& name = "", json** values = nullptr, ull size = 0);
	~json_array();

	std::string stringValue() const;

	json& value(int);
	json& operator[](int);

	type getChildType();
	void changeType(type);
	ull getSize();
	void resize(ull);
};

