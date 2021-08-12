#pragma once
#include "json_base.h"

class json_int : public json
{
	int v;

public:
	json_int(const std::string& name = "", int value = 0);

	std::string stringValue() const;

	int& iValue();
	operator int& ();
	void operator=(int);
};

