#pragma once
#include "../nbt.h"
#include "varData.h"

class Slot
{
private:
	bool present = false;
	varInt itemId = 0;
	Byte count = 0;
	nbt* nbt_data;

public:
	Slot() {
		nbt_data = new nbt_compound();
	}

	Slot(bool present, varInt itemId, Byte count = 0, nbt* nbt_data = new nbt_compound()) : present(present), itemId(itemId), count(count), nbt_data(nbt_data){
		
	}

	~Slot() {
		delete nbt_data;
	}

	void write(char*& buffer) const;

	bool isPresent() { return present; }
	varInt getItemId() { return itemId; }
	Byte getCount() { return count; }

	void updateNBT(nbt* newData) {
		delete nbt_data;
		nbt_data = newData;
	}
};