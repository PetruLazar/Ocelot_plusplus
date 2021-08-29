#pragma once
#include "json_base.h"

class json_boolean : public json
{
	bool v;

public:
	JSON_API json_boolean(const std::string& name = "", bool value = false);

	JSON_API std::string stringValue() const;

	JSON_API bool& bValue();
	JSON_API operator bool& ();

	void JSON_API operator=(bool);
	void JSON_API operator=(int);
};

