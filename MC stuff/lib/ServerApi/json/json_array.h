#pragma once
#include "json_base.h"

class json_array : public json
{
	ull s;
	json** v;
	type childType;

public:
	SERVER_API json_array(const std::string& name = "", json** values = nullptr, ull size = 0);
	SERVER_API ~json_array();

	SERVER_API std::string stringValue() const;

	SERVER_API json& value(int);
	SERVER_API json& operator[](int);

	SERVER_API type getChildType();
	void SERVER_API changeType(type);
	SERVER_API ull getSize();
	void SERVER_API resize(ull);
};

