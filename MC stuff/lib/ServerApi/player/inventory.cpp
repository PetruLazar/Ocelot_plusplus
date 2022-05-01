
#include "inventory.h"
#include "../types/item.h"

namespace mcp {
	inventory::inventory()
	{
		for (int i = 0; i < 46; i++)
			slots[i] = new Slot();
	}

	inventory::~inventory()
	{
		for (int i = 0; i < 46; i++)
			delete slots[i];

		delete floatingItem;
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

	Slot*& inventory::getInventorySlot(bshort index)
	{
		return this->slots[index];
	}

	Slot*& inventory::getFloatingSlot()
	{
		return this->floatingItem;
	}
	void inventory::setFloatingSlot(Slot* newSlot)
	{
		delete this->floatingItem;
		this->floatingItem = newSlot;
	}

	Byte inventory::getSlotWithLeastID(varInt itemID)
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
	Byte inventory::getFreeSlot()
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

	unsigned inventory::add(Slot& theItem, unsigned& addedIndex)
	{
		Byte picked = 0, index, stackableSize = items::getStackableSize(theItem.getItemId());

		if (stackableSize == 1) { //firstly, check if the item can be stacked at all
			index = this->getFreeSlot();

			if (index != 255) {
				picked = theItem.count;

				this->setInventorySlot(index, new Slot(theItem));

				addedIndex = index;
			}
		}
		else { //if the item can be stacked...
			index = this->getSlotWithLeastID(theItem.getItemId()); //find a slot with the least amount of it

			if (index != 255) { //found a slot with that item
				Slot*& containedSlot = this->getInventorySlot(index);

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

					this->setInventorySlot(index, new Slot(theItem));

					addedIndex = index;
				}
			}
		}

		return picked;
	}
	void inventory::swapSlots(bshort a, bshort b)
	{
		std::swap(this->getInventorySlot(a), this->getInventorySlot(b));
	}

	void inventory::setInventorySlot(bshort index, Slot* slot)
	{
		delete this->slots[index];
		this->slots[index] = slot;
	}
}
