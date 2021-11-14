#pragma once

#include "slot.h"
#include "item.h"

class Equipment {
private:
	Byte slot;
	Slot* item;

public:
	Equipment(Byte slot, Slot* item) : slot(slot), item(item) {
		slot |= 1 << 0x80;
	}

	void unSet() {
		slot &= ~(1 << 0x80);
	}

	void write(char*& buffer) const;

	Byte getSlot() { return slot; }
};