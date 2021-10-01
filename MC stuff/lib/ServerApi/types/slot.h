#pragma once
#include "../nbt.h"
#include "varData.h"

class Slot
{
private:
	bool present = false;
	varInt itemId = 0;
	Byte count = 0;
	nbt_compound data;

public:
	Slot() {

	}

	Slot(bool present, varInt itemId, Byte count = 0, nbt_compound data = nbt_compound()) : present(present), itemId(itemId), count(count), data(data){
		
	}

	void write(char*& buffer) const;
};