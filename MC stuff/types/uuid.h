#pragma once
#include "typedefs.h"

class mcUUID
{
	ull data1, data2;

public:
	void write(char*&) const;
};

