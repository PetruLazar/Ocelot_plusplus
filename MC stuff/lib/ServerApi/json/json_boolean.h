#pragma once
#include "json_base.h"

class json_boolean : public json
{
	bool v;

public:
	SERVER_API json_boolean(const std::string& name = "", bool value = false);

	SERVER_API std::string stringValue() const;

	SERVER_API bool& bValue();
	SERVER_API operator bool& ();

	void SERVER_API operator=(bool);
	void SERVER_API operator=(int);
};

