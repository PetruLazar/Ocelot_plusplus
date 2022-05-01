
#include "inventory.h"
#include "../types/item.h"
#include "../server/log.h"

namespace mcs::inventory {
	base::base(size_t size) : size(size) {
		slots = new Slot * [size];
		for (size_t i = 0; i < size; i++)
			slots[i] = new Slot();
	}

	base::~base() {
		for (size_t i = 0; i < size; i++)
			delete slots[i];
		delete[] slots;
	}

	size_t base::getSize() {
		return size;
	}

	Byte base::getSlotWithLeastID(varInt itemID)
	{
		Byte indexMin = 255, minCount = 255;
		for (int i = 36; i < 45; i++) {
			if (slots[i]->getItemId() == itemID && slots[i]->count != 64 && slots[i]->count < minCount) { //change 64 to item maximum regarding to that item
				minCount = slots[i]->count;
				indexMin = i;
			}
		}

		for (int i = 9; i < 36; i++) {
			if (slots[i]->getItemId() == itemID && slots[i]->count != 64 && slots[i]->count < minCount) { //change 64 to item maximum regarding to that item
				minCount = slots[i]->count;
				indexMin = i;
			}
		}

		return indexMin;
	}
	Byte base::getFreeSlot()
	{
		for (int i = 36; i < 45; i++) {
			if (!this->slots[i]->isPresent())
				return i;
		}

		for (int i = 9; i < 36; i++) {
			if (!this->slots[i]->isPresent())
				return i;
		}

		return -1;
	}

	void base::swapSlots(bshort a, bshort b)
	{
		std::swap(getSlotByIndex(a), getSlotByIndex(b));
	}

	void base::setSlotByIndex(bshort index, const Slot& slot)
	{
		*this->slots[index] = slot;
	}
	Slot*& base::getSlotByIndex(bshort index)
	{
		return this->slots[index];
	}
}

namespace mcp {
	inventory::~inventory() {
		delete floatingItem;
	}

	unsigned inventory::openWindow(window::type theWindow)
	{
		windowQue.emplace(std::make_pair(theWindow, windowIndex));
		isWindowOpen = true;

		switch (theWindow) {
		case window::type::crafting:
			openedWindowInventory = new mcs::inventory::crafting();
			break;
		default:

			break;
		}

		inventoryFirstSlotIndex = openedWindowInventory->getSize();

		return windowIndex++;
	}
	void inventory::closeWindow(unsigned theID)
	{
		delete openedWindowInventory;
		isWindowOpen = false;
		inventoryFirstSlotIndex = 9;

		if (theID == windowQue.front().second)
		{
			//close current window
			windowQue.pop();
			windowIndex--;
		}
		else
			Log::warn() << "Player closed a not-last-opened window." << Log::endl;
	}
	window::type inventory::getLatestWindow(unsigned theID)
	{
		if (theID == windowQue.back().second)
			return windowQue.back().first;

		Log::warn() << "Player got a wrong ID latest." << Log::endl;
		return windowQue.back().first;
	}

	void inventory::setSelectedSlot(Byte selectedSlot)
	{
		this->selectedHotbar = selectedSlot;
	}
	Byte inventory::getSelectedIndex(bool raw)
	{
		if (!raw)
			return this->selectedHotbar;

		return 36 + this->selectedHotbar;
	}

	Slot*& inventory::getSelectedSlot()
	{
		return this->slots[36 + selectedHotbar];
	}
	Slot*& inventory::getOffhandSlot()
	{
		return this->slots[45];
	}
	Slot*& inventory::getHotbarSlot(bshort index)
	{
		return this->slots[36 + index];
	}

	void inventory::swapWithFloating(bshort index)
	{
		std::swap(getFloatingSlot(), getSlotByIndex(index));
	}

	Slot*& inventory::getFloatingSlot()
	{
		return this->floatingItem;
	}
	void inventory::setFloatingSlot(const Slot& newSlot)
	{
		*this->floatingItem = newSlot;
	}

	bool inventory::isIndexLocal(bshort index)
	{
		return index >= inventoryFirstSlotIndex;
	}
	void inventory::swapSlots(bshort a, bshort b)
	{
		std::swap(getSlotByIndex(a), getSlotByIndex(b));
	}
	void inventory::setSlotByIndex(bshort index, const Slot& slot)
	{
		if(!isWindowOpen)
			*this->slots[index + 9 - inventoryFirstSlotIndex] = slot;
		else if (isIndexLocal(index))
			*this->slots[index + 9 - inventoryFirstSlotIndex] = slot;
		else
			this->openedWindowInventory->setSlotByIndex(index, slot);
	}
	Slot*& inventory::getSlotByIndex(bshort index)
	{
		if(!isWindowOpen)
			return this->slots[index + 9 - inventoryFirstSlotIndex];

		if (isIndexLocal(index))
			return this->slots[index + 9 - inventoryFirstSlotIndex];

		return this->openedWindowInventory->getSlotByIndex(index);
	}

	unsigned inventory::addAnywhere(Slot& theItem, unsigned& addedIndex)
	{
		Byte picked = 0, index, stackableSize = items::getStackableSize(theItem.getItemId());

		if (stackableSize == 1) { //firstly, check if the item can be stacked at all
			index = this->getFreeSlot();

			if (index != 255) {
				picked = theItem.count;
				addedIndex = index;
			}
		}
		else { //if the item can be stacked...
			index = this->getSlotWithLeastID(theItem.getItemId()); //find a slot with the least amount of it

			if (index != 255) { //found a slot with that item
				Slot* containedSlot = this->getSlotByIndex(index);

				if (containedSlot->count + theItem.count < stackableSize + 1) { //the stash can be completely picked up
					picked = theItem.count;
					containedSlot->count = containedSlot->count + theItem.count;
				}
				else { //not the entire stash can be pickedup...
					picked = stackableSize - containedSlot->count;
					containedSlot->count = stackableSize;
				}

				addedIndex = index;
			}
			else { //no slot with the same item, putting in a new one
				index = this->getFreeSlot(); //if this fails, there are no free slots, therefore cant pick the item up

				if (index != 255) {
					picked = theItem.count;
					addedIndex = index;
				}
			}
		}

		return picked;
	}
	unsigned inventory::addToSlot(Slot* theItem, bshort index)
	{
		Byte picked = 0, stackableSize = items::getStackableSize(theItem->getItemId());

		Slot* containedSlot = this->getSlotByIndex(index);

		if (containedSlot->count + theItem->count < stackableSize + 1) { //the stash can be completely picked up
			picked = theItem->count;
			containedSlot->count = containedSlot->count + theItem->count;
		}
		else { //not the entire stash can be pickedup...
			picked = stackableSize - containedSlot->count;
			containedSlot->count = stackableSize;
		}

		return picked;
	}
}
