#include <gtest/gtest.h>

#include "player/inventory.h"

TEST(BaseInventory, GetSize) {
	mcs::inventory::base inventory(20);

	EXPECT_EQ(inventory.getSize(), 20);
}

TEST(BaseInventory, SetGetSlot) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(10, Slot(440));
	Slot* slot = inventory.getSlotByIndex(10);
	EXPECT_EQ(slot->getItemId(), 440);
	EXPECT_EQ(slot->count, 1);
}

TEST(BaseInventory, GetSlotWithID) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(10, Slot(440));

	EXPECT_EQ(inventory.getSlotIndexWithID(440), 10);
}

TEST(BaseInventory, GetStackableSlotWithID) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(10, Slot(440));

	EXPECT_EQ(inventory.getStackableSlotIndexWithID(440), 10);
}

TEST(BaseInventory, GetFreeSlot) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(0, Slot(440));
	inventory.setSlotByIndex(1, Slot(440));

	EXPECT_EQ(inventory.getFreeSlotIndex(), 2);
}

TEST(BaseInventory, SwapSlot) {
	mcs::inventory::base inventory(50);

	inventory.setSlotByIndex(30, Slot(440));
	inventory.setSlotByIndex(40, Slot(420));

	inventory.swapSlots(30, 40);

	EXPECT_EQ(inventory.getSlotByIndex(30)->getItemId(), 420);
	EXPECT_EQ(inventory.getSlotByIndex(40)->getItemId(), 440);
}

TEST(BaseInventory, StackItem) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(10, Slot(440));
	Slot item(440);
	inventory.stackItem(&item);

	EXPECT_EQ(item.count, 2);
	EXPECT_EQ(inventory.getSlotByIndex(10)->isPresent(), false);
}

TEST(BaseInventory, AddAnywhere) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(16, Slot(440));

	Slot item(440);
	auto data = inventory.addAnywhere(item);

	EXPECT_EQ(data.first, 1);
	EXPECT_EQ(data.second, 16);

	item = Slot(441);
	data = inventory.addAnywhere(item);

	EXPECT_EQ(data.first, 1);
	EXPECT_EQ(data.second, 0);
}
TEST(BaseInventory, AddToInventory) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(10, Slot(1062, 9));
	inventory.setSlotByIndex(11, Slot(1062, 15));
	inventory.setSlotByIndex(13, Slot(1062, 15));

	Slot item(1062, 16); //honey bottle, stackable to 16

	auto data = inventory.addToInventory(item);
	EXPECT_EQ(data.size(), 4);

	EXPECT_EQ(data[0].first, 7);
	EXPECT_EQ(data[0].second, 10);
	EXPECT_EQ(data[1].first, 1);
	EXPECT_EQ(data[1].second, 11);
	EXPECT_EQ(data[2].first, 1);
	EXPECT_EQ(data[2].second, 13);
	EXPECT_EQ(data[3].first, 7);
	EXPECT_EQ(data[3].second, 0);

	EXPECT_EQ(inventory.getSlotByIndex(10)->count, 16);
	EXPECT_EQ(inventory.getSlotByIndex(11)->count, 16);
	EXPECT_EQ(inventory.getSlotByIndex(0)->count, 7);
	EXPECT_EQ(inventory.getSlotByIndex(13)->count, 16);
}
TEST(BaseInventory, AddToSlot) {
	mcs::inventory::base inventory(20);

	inventory.setSlotByIndex(0, Slot(440));

	Slot item(440);
	Byte picked = inventory.addToSlot(item, 0);

	EXPECT_EQ(picked, 1);
	EXPECT_EQ(inventory.getSlotByIndex(0)->count, 2);

	item.count = 64;
	picked = inventory.addToSlot(item, 0);

	EXPECT_EQ(picked, 62);
	EXPECT_EQ(inventory.getSlotByIndex(0)->count, 64);
}