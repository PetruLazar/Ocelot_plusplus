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
		SERVER_API base(size_t size);
		SERVER_API ~base();

		SERVER_API size_t getSize();

		SERVER_API Byte getSlotIndexWithID(varInt itemID);
		SERVER_API Byte getStackableSlotIndexWithID(varInt itemID);
		SERVER_API Byte getFreeSlotIndex();

		SERVER_API void swapSlots(bshort a, bshort b);

		SERVER_API void setSlotByIndex(bshort index, const Slot& slot);
		SERVER_API Slot*& getSlotByIndex(bshort index);

		SERVER_API std::pair<Byte, Byte> addAnywhere(const Slot& theItem); //first is the amount added and second the index
		SERVER_API std::vector<std::pair<Byte, Byte>> addToInventory(const Slot& theItem);
		SERVER_API Byte addToSlot(const Slot& theItem, bshort index);
		SERVER_API virtual bool stackItem(Slot* theItem);
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
		Byte inventoryFirstSlotIndex = 9;

		std::queue<std::pair<window::type, unsigned>> windowQue;
		unsigned windowIndex = 1;

		bool isIndexLocal(bshort index);
		Byte getTrueIndex(Byte index);

	public:
		SERVER_API inventory() : mcs::inventory::base(46) { };
		SERVER_API ~inventory();

		SERVER_API unsigned openWindow(window::type theWindow);
		SERVER_API void closeWindow(unsigned ID);
		SERVER_API window::type getLatestWindow(unsigned ID);

		SERVER_API void setSelectedIndex(Byte selectedSlot);
		SERVER_API Byte getSelectedIndex(bool raw = false);
		SERVER_API Slot*& getSelectedSlot();
		SERVER_API void setSelectedSlot(const Slot& slot);

		SERVER_API void setOffhandSlot(const Slot& slot);
		SERVER_API Slot*& getOffhandSlot();

		SERVER_API void setHotbarSlot(Byte index, const Slot& slot);
		SERVER_API Slot*& getHotbarSlot(Byte index);

		SERVER_API Byte getSlotIndexWithID(varInt itemID);
		SERVER_API Byte getStackableSlotIndexWithID(varInt itemID);
		SERVER_API Byte getFreeSlotIndex();

		SERVER_API void swapWithFloating(bshort index);
		SERVER_API Slot*& getFloatingSlot();
		SERVER_API void setFloatingSlot(const Slot& newSlot);

		SERVER_API void swapSlots(bshort a, bshort b);
		SERVER_API void setSlotByIndex(bshort index, const Slot& slot);
		SERVER_API Slot*& getSlotByIndex(bshort index);

		SERVER_API std::pair<Byte, Byte> addAnywhere(const Slot& theItem); //first is the amount added and second the index
		SERVER_API std::vector<std::pair<Byte, Byte>> addToInventory(const Slot& theItem);
		SERVER_API Byte addToSlot(const Slot& theItem, bshort index);

		SERVER_API virtual bool stackItem(Slot* theItem) override;
	};
}
