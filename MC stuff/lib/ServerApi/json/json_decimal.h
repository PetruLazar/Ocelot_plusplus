#pragma once
#include "json_base.h"

class json_decimal : public json
{
	double v;

public:
	SERVER_API json_decimal(const std::string& name = "", double value = 0.0);

	SERVER_API std::string stringValue() const;

	SERVER_API double& dValue();
	SERVER_API const double& dValue() const;
	SERVER_API operator double& ();
	SERVER_API operator const double& () const;
	void SERVER_API operator=(double);
};

#pragma once
