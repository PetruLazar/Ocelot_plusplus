#pragma once
#include "json_base.h"

class json_int : public json
{
	int v;

public:
	SERVER_API json_int(const std::string& name = "", int value = 0);

	SERVER_API std::string stringValue() const;

	SERVER_API int& iValue();
	SERVER_API operator int& ();
	void SERVER_API operator=(int);
};

