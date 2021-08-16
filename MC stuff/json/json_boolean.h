#pragma once
#include "json_base.h"

class json_boolean : public json
{
	bool v;

public:
	json_boolean(const std::string& name = "", bool value = false);

	std::string stringValue() const;

	bool& bValue();
	operator bool& ();

	void operator=(bool);
	void operator=(int);
};

