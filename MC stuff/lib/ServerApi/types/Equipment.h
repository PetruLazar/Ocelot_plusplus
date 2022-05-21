#pragma once

#include "slot.h"
#include "item.h"

class Equipment {
private:
	Byte slot;
	Slot* item;

public:
	enum class Type { MainHand, OffHand, Armor_boots, Armor_leggings, Armor_chestplate, Armor_helmet };

	Equipment(Type type, Slot* item) : slot(static_cast<Byte>(type)), item(item) {
		this->slot |= 0x80;
	}

	void unSet() { //remember to do this on the last element in array
		slot &= ~(0x80);
	}

	void write(char*& buffer) const;

	Byte getSlot() { return slot; }
};