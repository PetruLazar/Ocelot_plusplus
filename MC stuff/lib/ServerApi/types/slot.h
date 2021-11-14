#pragma once
#include "../nbt.h"
#include "varData.h"
#include <iostream>

class Slot
{
private:
	bool present = false;
	varInt itemId = 0;
	nbt* nbt_data;

public:
	Byte count = 0;

	Slot() {
		nbt_data = new nbt_compound();
	}

	Slot(bool present, varInt itemId, Byte count = 0, nbt* nbt_data = new nbt_compound()) : present(present), itemId(itemId), count(count), nbt_data(nbt_data){
		
	}

	Slot(const Slot& s) : present(s.present), itemId(s.itemId), count(s.count), nbt_data(new nbt_compound()) {
		*nbt_data = *s.nbt_data;
	}

	~Slot() {
		delete nbt_data;
	}

	void write(char*& buffer) const;

	bool isPresent() { return present; }
	varInt getItemId() { return itemId; }

	void updateNBT(nbt* newData) {
		delete nbt_data;
		nbt_data = new nbt_compound();
		*nbt_data = *newData;
	}
};