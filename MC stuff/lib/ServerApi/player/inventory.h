#pragma once

#include "../types/slot.h"
#include "../types/typedefs.h"

namespace mcp {
	class inventory {
	private:
		Slot* slots[46];
		Byte selectedHotbar = 0; //main hand selected slot
		Slot* floatingItem = new Slot();

	public:
		inventory();
		~inventory();

		void setSelectedSlot(Byte selectedSlot);
		Byte getSelectedIndex(bool raw = false);

		Slot*& getSelectedSlot();
		Slot*& getOffhandSlot();
		Slot*& getHotbarSlot(bshort index);
		Slot*& getInventorySlot(bshort index);

		Slot*& getFloatingSlot();
		void setFloatingSlot(Slot* newSlot);

		Byte getSlotWithLeastID(varInt itemID);
		Byte getFreeSlot();

		unsigned add(Slot& theItem, unsigned& addedIndex);
		void swapSlots(bshort a, bshort b);

		void setInventorySlot(bshort index, Slot* slot);
	};
}
