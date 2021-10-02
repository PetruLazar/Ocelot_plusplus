#pragma once
#include "../nbt.h"
#include "varData.h"

#include <iostream>

class Slot
{
private:
	bool present = false;
	varInt itemId = 0;
	Byte count = 0;
	nbt_compound* nbt_data;

public:
	Slot() {
		nbt_data = new nbt_compound();
	}

	Slot(bool present, varInt itemId, Byte count = 0, nbt_compound* nbt_data = new nbt_compound()) : present(present), itemId(itemId), count(count), nbt_data(nbt_data){
		
	}

	~Slot() {
		delete nbt_data;
	}

	void write(char*& buffer) const;
};