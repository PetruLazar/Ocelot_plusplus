#pragma once
#include "../basic.h"

class json_base
{
public:
	enum type : byte
	{
		integer,
		string,
		array,
		compound
	};

	json_base(type);
	json_base(type, std::string);

	std::string getName();
	type getType();

	virtual int iValue();
	virtual std::string value();
	virtual json_base* value(int);
	virtual json_base* value(std::string);

	virtual operator std::string() = 0;

private:
	type t;
	std::string name;
};

