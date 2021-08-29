#pragma once
#include "json_base.h"

class json_array : public json
{
	ull s;
	json** v;
	type childType;

public:
	JSON_API json_array(const std::string& name = "", json** values = nullptr, ull size = 0);
	JSON_API ~json_array();

	JSON_API std::string stringValue() const;

	JSON_API json& value(int);
	JSON_API json& operator[](int);

	JSON_API type getChildType();
	void JSON_API changeType(type);
	JSON_API ull getSize();
	void JSON_API resize(ull);
};

