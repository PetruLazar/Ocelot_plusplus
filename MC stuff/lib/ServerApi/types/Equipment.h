#pragma once

#include "slot.h"
#include "item.h"

#include <iostream>

class Equipment {
private:
	Byte slot;
	Slot* item;

public:
	Equipment(Byte slot, Slot* item) : slot(slot), item(item) {

	}

	void write(char*& buffer) const;

	Byte getSlot() { return slot; }
};