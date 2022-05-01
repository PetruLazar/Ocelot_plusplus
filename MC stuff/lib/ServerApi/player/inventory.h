#pragma once

#include "../types/slot.h"
#include "../types/typedefs.h"
#include "../types/window.h"
#include <queue>

namespace mcs::inventory {
	class base
	{
	protected:
		Slot** slots;
		size_t size;

	public:
		base(size_t size);
		~base();

		size_t getSize();

		Byte getSlotWithLeastID(varInt itemID);
		Byte getFreeSlot();

		virtual void swapSlots(bshort a, bshort b);

		virtual void setSlotByIndex(bshort index, const Slot& slot);
		virtual Slot*& getSlotByIndex(bshort index);
	};

	class crafting : public base
	{
	public:
		crafting() : base(10) {};
	};
}

namespace mcp {
	class inventory : public mcs::inventory::base {
	private:
		Byte selectedHotbar = 0; //main hand selected slot
		Slot* floatingItem = new Slot();

		mcs::inventory::base* openedWindowInventory = nullptr;
		bool isWindowOpen = false;
		unsigned inventoryFirstSlotIndex = 9;

		std::queue<std::pair<window::type, unsigned>> windowQue;
		unsigned windowIndex = 1;

	public:
		inventory() : mcs::inventory::base(46) { };
		~inventory();

		unsigned openWindow(window::type theWindow);
		void closeWindow(unsigned ID);
		window::type getLatestWindow(unsigned ID);

		void setSelectedSlot(Byte selectedSlot);
		Byte getSelectedIndex(bool raw = false);

		Slot*& getSelectedSlot();
		Slot*& getOffhandSlot();
		Slot*& getHotbarSlot(bshort index);

		void swapWithFloating(bshort index);

		Slot*& getFloatingSlot();
		void setFloatingSlot(const Slot& newSlot);

		bool isIndexLocal(bshort index);
		void swapSlots(bshort a, bshort b) override;
		void setSlotByIndex(bshort index, const Slot& slot) override;
		Slot*& getSlotByIndex(bshort index) override;

		unsigned addAnywhere(Slot& theItem, unsigned& addedIndex);
		unsigned addToSlot(Slot* theItem, bshort index);
	};
}
