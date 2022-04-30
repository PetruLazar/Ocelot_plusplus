#pragma once

#include "slot.h"
#include "item.h"

class Equipment {
private:
	Byte slot;
	Slot* item;

public:
	Equipment(Byte slot, Slot* item) : slot(slot), item(item) {
		this->slot |= 0x80;
	}

	void unSet() { //remember to do this on the last element in array
		slot &= ~(0x80);
	}

	void write(char*& buffer) const;

	Byte getSlot() { return slot; }
};