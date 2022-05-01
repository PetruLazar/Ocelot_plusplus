#pragma once
#include "../nbt.h"
#include "varData.h"

class Slot
{
private:
	bool present;
	varInt itemId;
	nbt* nbt_data;

public:
	Byte count = 0;

	Slot() {
		nbt_data = new nbt_compound();
		present = false;
		itemId = 0;
	}

	Slot(varInt itemId, nbt* nbt_data = new nbt_compound())
		: present(true), itemId(itemId), count(1), nbt_data(nbt_data) {}

	Slot(varInt itemId, Byte count, nbt* nbt_data = new nbt_compound())
		: present(true), itemId(itemId), count(count), nbt_data(nbt_data) {}

	Slot(varInt itemId, bool present, Byte count, nbt* nbt_data = new nbt_compound())
		: itemId(itemId), present(present), count(count), nbt_data(nbt_data) {}

	Slot(const Slot& s) : present(s.present), itemId(s.itemId), count(s.count), nbt_data(new nbt_compound()) {
		*nbt_data = *s.nbt_data;
	}

	~Slot() {
		delete nbt_data;
	}

	void write(char*& buffer) const;
	void read(char*& buffer);

	bool isPresent() { return present; }
	varInt getItemId() { return itemId; }

	void updateNBT(nbt* newData) {
		delete nbt_data;
		nbt_data = new nbt_compound();
		*nbt_data = *newData;
	}

	Slot& operator=(const Slot& other) {
		count = other.count;
		itemId = other.itemId;
		present = other.present;
		this->updateNBT(other.nbt_data);

		return *this;
	}
};