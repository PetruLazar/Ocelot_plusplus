#pragma once
#include "json_base.h"

class json_int : public json
{
	int v;

public:
	SERVER_API json_int(const std::string& name = "", int value = 0);

	SERVER_API std::string stringValue() const;

	SERVER_API int& iValue();
	SERVER_API const int& iValue() const;
	SERVER_API operator int& ();
	SERVER_API operator const int& () const;
	void SERVER_API operator=(int);
};

