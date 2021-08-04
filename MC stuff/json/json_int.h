#pragma once
#include "json_base.h"

class json_int : public json
{
	int v;

public:
	json_int(const std::string& name);
	json_int(const std::string& name, int value);

	std::string stringValue();

	int& iValue();
	operator int& ();
	void operator=(int);
};

