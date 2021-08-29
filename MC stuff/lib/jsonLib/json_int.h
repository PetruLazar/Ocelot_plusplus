#pragma once
#include "json_base.h"

class json_int : public json
{
	int v;

public:
	JSON_API json_int(const std::string& name = "", int value = 0);

	JSON_API std::string stringValue() const;

	JSON_API int& iValue();
	JSON_API operator int& ();
	void JSON_API operator=(int);
};

