#pragma once
#include "../nbt.h"
#include "../json.h"
#include "../types.h"

class Slot
{
private:
	bool present;
	varInt itemId;
	Byte count;
	nbt_compound data;
public:
	Slot(bool present, varInt itemId, Byte count = 0, nbt_compound data = nbt_compound()) : present(present), itemId(itemId), count(count), data(data){
		
	}

	void write(char*& buffer) const;
};