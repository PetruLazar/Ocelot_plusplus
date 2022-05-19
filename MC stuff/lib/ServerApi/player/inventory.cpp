
#include "inventory.h"
#include "../types/item.h"
#include "../debug/log.h"

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

	Byte base::getSlotIndexWithID(varInt itemID)
	{
		for (int i = 0; i < size; i++) {
			if (slots[i]->getItemId() == itemID)
				return i;
		}

		return 255;
	}
	Byte base::getStackableSlotIndexWithID(varInt itemID)
	{
		for (int i = 0; i < size; i++) {
			if (slots[i]->getItemId() == itemID && slots[i]->count != Slot::getStackableSize(slots[i]))
				return i;
		}

		return 255;
	}
	Byte base::getFreeSlotIndex()
	{
		for (int i = 0; i < size; i++) {
			if (!this->slots[i]->isPresent())
				return i;
		}

		return 255;
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

	std::pair<Byte, Byte> base::addAnywhere(const Slot& theItem)
	{
		Byte picked = 0, index, stackableSize = Slot::getStackableSize(theItem);

		if (stackableSize == 1) { //firstly, check if the item can be stacked at all
			index = this->getFreeSlotIndex();

			if (index != 255) {
				picked = theItem.count;
				this->setSlotByIndex(index, theItem);
			}
		}
		else { //if the item can be stacked...
			index = this->getStackableSlotIndexWithID(theItem.getItemId()); //find a slot with the same ID

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
			}
			else { //no slot with the same item, putting in a new one
				index = this->getFreeSlotIndex(); //if this fails, there are no free slots, therefore cant pick the item up

				if (index != 255) {
					picked = theItem.count;
					this->setSlotByIndex(index, theItem);
				}
			}
		}

		return std::make_pair(picked, index);
	}
	std::vector<std::pair<Byte, Byte>> base::addToInventory(const Slot& theItem)
	{
		std::vector<std::pair<Byte, Byte>> pickedDataArray;
		std::pair<Byte, Byte> picked;

		Slot* processingItem = new Slot(theItem);

		bool exit = false;
		while (!exit) {
			picked = this->addAnywhere(*processingItem);

			if (picked.first == 0)
				exit = true;
			else {
				pickedDataArray.emplace_back(picked);
				processingItem->count -= picked.first;

				exit = processingItem->count == 0;
			}
		}

		delete processingItem;

		return pickedDataArray;
	}
	Byte base::addToSlot(const Slot& theItem, bshort index)
	{
		Slot* containedSlot = this->getSlotByIndex(index);

		if (!containedSlot->isPresent()) {
			*containedSlot = theItem;
			return theItem.count;
		}
		else if (containedSlot->getItemId() != theItem.getItemId())
			return 0;

		Byte picked = 0, stackableSize = Slot::getStackableSize(theItem);

		if (containedSlot->count + theItem.count < stackableSize + 1) { //the stash can be completely picked up
			picked = theItem.count;
			containedSlot->count = containedSlot->count + theItem.count;
		}
		else { //not the entire stash can be pickedup...
			picked = stackableSize - containedSlot->count;
			containedSlot->count = stackableSize;
		}

		return picked;
	}
	bool base::stackItem(Slot* theItem)
	{
		Byte clickedStackableSize = Slot::getStackableSize(theItem);
		if (clickedStackableSize == 1)
			return false;

		Byte foundSlotIndex = this->getSlotIndexWithID(theItem->getItemId()), addedToItem = 0;
		while (foundSlotIndex != 255 && theItem->count != clickedStackableSize)
		{
			Slot* foundSlot = this->getSlotByIndex(foundSlotIndex);
			Byte tookFromSlot = 0;

			if (theItem->count + foundSlot->count > clickedStackableSize)
			{
				tookFromSlot = (theItem->count + foundSlot->count) - clickedStackableSize;
				foundSlot->count -= tookFromSlot;
			}
			else
			{
				tookFromSlot = foundSlot->count;
				this->setSlotByIndex(foundSlotIndex, Slot());
			}

			theItem->count += tookFromSlot;
			addedToItem += tookFromSlot;

			foundSlotIndex = this->getSlotIndexWithID(theItem->getItemId());
		}

		return addedToItem != 0;
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
		case window::type::generic_9x1:

			break;
		case window::type::generic_9x2:

			break;
		case window::type::generic_9x3:

			break;
		case window::type::generic_9x4:

			break;
		case window::type::generic_9x5:

			break;
		case window::type::generic_9x6:

			break;
		case window::type::generic_3x3:

			break;
		case window::type::anvil:

			break;
		case window::type::beacon:

			break;
		case window::type::blast_furnace:

			break;
		case window::type::brewing_stand:

			break;
		case window::type::crafting:
			openedWindowInventory = new mcs::inventory::crafting();
			break;
		case window::type::enchantment:

			break;
		case window::type::furnace:
			openedWindowInventory = new mcs::inventory::furnace();
			break;
		case window::type::grindstone:

			break;
		case window::type::hopper:

			break;
		case window::type::lectern:

			break;
		case window::type::loom:

			break;
		case window::type::merchant:

			break;
		case window::type::shulker_box:

			break;
		case window::type::smithing:

			break;
		case window::type::smoker:

			break;
		case window::type::cartography:

			break;
		case window::type::stonecutter:

			break;
		}

		inventoryFirstSlotIndex = (Byte)openedWindowInventory->getSize();

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

	bool inventory::paintProcessLeft(bshort* slotNumbers, Byte slotsCount)
	{
		Slot* floatingSlot = this->getFloatingSlot();
		Byte remainedAmount = floatingSlot->count % slotsCount;

		floatingSlot->count = floatingSlot->count / slotsCount;
		for (Byte i = 0; i < slotsCount; i++)
			this->setSlotByIndex(slotNumbers[i], *floatingSlot);
		
		if (remainedAmount == 0)
			this->setFloatingSlot(Slot());
		else
			floatingSlot->count = remainedAmount;

		return true;
	}
	bool inventory::paintProcessMiddle(bshort* slotNumbers, Byte slotsCount)
	{
		Slot floatingSlot = *this->getFloatingSlot();
		floatingSlot.count = Slot::getStackableSize(floatingSlot);

		for (Byte i = 0; i < slotsCount; i++)
			this->setSlotByIndex(slotNumbers[i], floatingSlot);

		this->setFloatingSlot(Slot());

		return true;
	}
	bool inventory::paintProcessRight(bshort* slotNumbers, Byte slotsCount)
	{
		Slot* floatingSlot = this->getFloatingSlot();
		Byte remainedAmount = floatingSlot->count - slotsCount;

		floatingSlot->count = 1;
		for (Byte i = 0; i < slotsCount; i++)
			this->setSlotByIndex(slotNumbers[i], *floatingSlot);
		
		if (remainedAmount == 0)
			this->setFloatingSlot(Slot());
		else
			floatingSlot->count = remainedAmount;

		return true;
	}

	bool inventory::paintStart(inventoryPaint side)
	{
		if (paintStarted)
		{
			Log::warn() << "Player sent paint start in wrong order." << Log::endl;
			paintReset();
			return false;
		}

		paintSide = side;
		paintStarted = true;
		return true;
	}
	void inventory::paintReset()
	{
		paintStarted = false;
		paintList.clear();
	}
	bool inventory::paintProgress(inventoryPaint side, Byte slotIndex)
	{
		if (!paintStarted || paintSide != side)
		{
			Log::warn() << "Player sent paint progress in wrong order." << Log::endl;
			paintReset();
			return false;
		}

		paintList.emplace_back(slotIndex);

		return true;
	}
	bool inventory::paintStop(inventoryPaint side, bshort* slotNumbers, Byte slotsCount)
	{
		if (!paintStarted || paintSide != side) {
			Log::warn() << "Player sent paint stop in wrong order." << Log::endl;
			paintReset();
			return false;
		}

		for (Byte i = 0; i < slotsCount; i++) {
			auto foundSlot = std::find(paintList.begin(), paintList.end(), slotNumbers[i]);
			if (foundSlot == paintList.end()) {
				Log::warn() << "Player sent paint stop with bad slots." << Log::endl;
				paintReset();
				return false;
			}
			else
				paintList.erase(foundSlot);
		}

		if (paintList.size()) {
			Log::warn() << "Player sent paint stop with missing slots." << Log::endl;
			paintReset();
			return false;
		}

		if (side == inventoryPaint::left) { //do sanitary checks?
			if (!paintProcessLeft(slotNumbers, slotsCount))
				return false;
		}
		else if (side == inventoryPaint::middle) {
			if (!paintProcessMiddle(slotNumbers, slotsCount))
				return false;
		}
		else { // if (side == inventoryPaint::right)
			if (!paintProcessRight(slotNumbers, slotsCount))
				return false;
		}

		paintReset();
		return true;
	}

	void inventory::setSelectedIndex(Byte selectedSlot)
	{
		this->selectedHotbar = selectedSlot;
	}
	Byte inventory::getSelectedIndex(bool raw)
	{
		if (!raw)
			return selectedHotbar;

		return getTrueIndex(36 + selectedHotbar);
	}

	Slot*& inventory::getSelectedSlot()
	{
		return getSlotByIndex(getTrueIndex(36 + selectedHotbar));
	}
	void inventory::setSelectedSlot(const Slot& slot)
	{
		this->setSlotByIndex(getSelectedIndex(true), slot);
	}

	void inventory::setOffhandSlot(const Slot& slot)
	{
		this->setSlotByIndex(getTrueIndex(45), slot);
	}
	Slot*& inventory::getOffhandSlot()
	{
		return getSlotByIndex(getTrueIndex(45));
	}

	void inventory::setHotbarSlot(Byte index, const Slot& slot)
	{
		this->setSlotByIndex(36 + index, slot);
	}
	Slot*& inventory::getHotbarSlot(Byte index)
	{
		return getSlotByIndex(36 + index);
	}

	Byte inventory::getSlotIndexWithID(varInt itemID)
	{
		for (int i = 36; i < 45; i++) {
			if (this->getSlotByIndex(i)->getItemId() == itemID)
				return i;
		}

		for (int i = 9; i < 36; i++) {
			if (this->getSlotByIndex(i)->getItemId() == itemID)
				return i;
		}

		if (isWindowOpen)
			return openedWindowInventory->getFreeSlotIndex();

		return 255;
	}
	Byte inventory::getStackableSlotIndexWithID(varInt itemID)
	{
		for (int i = 36; i < 45; i++) {
			Slot* slot = this->getSlotByIndex(i);
			if (slot->getItemId() == itemID && slot->count != Slot::getStackableSize(slot))
				return i;
		}

		for (int i = 9; i < 36; i++) {
			Slot* slot = this->getSlotByIndex(i);
			if (slot->getItemId() == itemID && slot->count != Slot::getStackableSize(slot))
				return i;
		}

		if(isWindowOpen)
			return openedWindowInventory->getFreeSlotIndex();

		return 255;
	}
	Byte inventory::getFreeSlotIndex()
	{
		for (int i = 36; i < 45; i++) {
			if (!this->getSlotByIndex(i)->isPresent())
				return i;
		}

		for (int i = 9; i < 36; i++) {
			if (!this->getSlotByIndex(i)->isPresent())
				return i;
		}

		if (isWindowOpen)
			return openedWindowInventory->getFreeSlotIndex();

		return 255;
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
	Byte inventory::getTrueIndex(Byte index)
	{
		if(isWindowOpen && isIndexLocal(index))
			return index + inventoryFirstSlotIndex - 9;

		return index;
	}

	void inventory::swapSlots(bshort a, bshort b)
	{
		std::swap(getSlotByIndex(a), getSlotByIndex(b));
	}
	void inventory::setSlotByIndex(bshort index, const Slot& slot)
	{
		if (!isWindowOpen)
			*this->slots[index] = slot;
		else {
			if (isIndexLocal(index))
				*this->slots[index + 9 - inventoryFirstSlotIndex] = slot;
			else
				this->openedWindowInventory->setSlotByIndex(index, slot);
		}
	}
	Slot*& inventory::getSlotByIndex(bshort index)
	{
		if(!isWindowOpen)
			return this->slots[index];
		else {
			if (isIndexLocal(index))
				return this->slots[index + 9 - inventoryFirstSlotIndex];
		}

		return this->openedWindowInventory->getSlotByIndex(index);
	}

	std::pair<Byte, Byte> inventory::addAnywhere(const Slot& theItem)
	{
		Byte picked = 0, index, stackableSize = Slot::getStackableSize(theItem);

		if (stackableSize == 1) { //firstly, check if the item can be stacked at all
			index = this->getFreeSlotIndex();

			if (index != 255) {
				picked = theItem.count;
				this->setSlotByIndex(index, theItem);
			}
		}
		else { //if the item can be stacked...
			Slot* selectedSlot = this->getSelectedSlot();

			//if the currently holding item is the adding item and can be picked up in it...
			if (selectedSlot->getItemId() == theItem.getItemId() && selectedSlot->count != Slot::getStackableSize(selectedSlot)) {
				if (selectedSlot->count + theItem.count < stackableSize + 1) { //the stash can be completely picked up
					picked = theItem.count;
					selectedSlot->count = selectedSlot->count + theItem.count;
				}
				else { //not the entire stash can be pickedup...
					picked = stackableSize - selectedSlot->count;
					selectedSlot->count = stackableSize;
				}

				index = this->getSelectedIndex(true);
			}
			else { //if the currently holding item is not the adding item...
				index = this->getStackableSlotIndexWithID(theItem.getItemId()); //find a slot with the same ID
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
				}
				else { //no slot with the same item, putting in a new one
					index = this->getFreeSlotIndex(); //if this fails, there are no free slots, therefore cant pick the item up

					if (index != 255) {
						picked = theItem.count;
						this->setSlotByIndex(index, theItem);
					}
				}
			}
		}

		return std::make_pair(picked, index);
	}
	std::vector<std::pair<Byte, Byte>> inventory::addToInventory(const Slot& theItem)
	{
		std::vector<std::pair<Byte, Byte>> pickedDataArray;
		std::pair<Byte, Byte> picked;

		Slot* processingItem = new Slot(theItem);

		bool exit = false;
		while (!exit) {
			picked = this->addAnywhere(*processingItem);

			if (picked.first == 0)
				exit = true;
			else {
				pickedDataArray.emplace_back(picked);
				processingItem->count -= picked.first;

				exit = processingItem->count == 0;
			}
		}

		delete processingItem;

		return pickedDataArray;
	}
	Byte inventory::addToSlot(const Slot& theItem, bshort index)
	{
		Slot* containedSlot = this->getSlotByIndex(index);

		if (!containedSlot->isPresent()) {
			*containedSlot = theItem;
			return theItem.count;
		}
		else if (containedSlot->getItemId() != theItem.getItemId())
			return 0;

		Byte picked = 0, stackableSize = Slot::getStackableSize(theItem);

		if (containedSlot->count + theItem.count < stackableSize + 1) { //the stash can be completely picked up
			picked = theItem.count;
			containedSlot->count = containedSlot->count + theItem.count;
		}
		else { //not the entire stash can be pickedup...
			picked = stackableSize - containedSlot->count;
			containedSlot->count = stackableSize;
		}

		return picked;
	}
	bool inventory::stackItem(Slot* theItem)
	{
		bool opened = false;
		if(isWindowOpen) //the opened inventory must pe processed first!
			opened = openedWindowInventory->stackItem(theItem);

		bool inv = base::stackItem(theItem);

		return opened || inv;
	}
}
