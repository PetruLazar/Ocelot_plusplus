#pragma once
#include "basic.h"

class mcUUID
{
	ull data1, data2;

public:
	void write(char*&) const;
};

