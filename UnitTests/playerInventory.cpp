#include <gtest/gtest.h>

#include "player/inventory.h"

TEST(PlayerInventory, SetGetSlot) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(39, Slot(440));
	Slot* slot = pInventory.getSlotByIndex(39);

	EXPECT_EQ(slot->getItemId(), 440);
	EXPECT_EQ(slot->count, 1);
}
TEST(PlayerInventory, SetGetSlot_WithWindow) {
	mcp::inventory pInventory;
	pInventory.openWindow(window::type::crafting);

	pInventory.setSlotByIndex(39, Slot(440));
	Slot* slot = pInventory.getSlotByIndex(39);

	EXPECT_EQ(slot->getItemId(), 440);
	EXPECT_EQ(slot->count, 1);

	pInventory.setSlotByIndex(2, Slot(440));
	Slot* windowSlot = pInventory.getSlotByIndex(2);

	EXPECT_EQ(slot->getItemId(), 440);
	EXPECT_EQ(slot->count, 1);
}

TEST(PlayerInventory, GetSlotWithID) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(10, Slot(440));

	EXPECT_EQ(pInventory.getSlotIndexWithID(440), 10);
}
TEST(PlayerInventory, GetStackableSlotWithID) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(10, Slot(440));

	EXPECT_EQ(pInventory.getStackableSlotIndexWithID(440), 10);
}
TEST(PlayerInventory, GetFreeSlot) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(0, Slot(440));
	pInventory.setSlotByIndex(36, Slot(440));

	EXPECT_EQ(pInventory.getFreeSlotIndex(), 37);
}

TEST(PlayerInventory, FloatingSlot) {
	mcp::inventory pInventory;
	
	pInventory.setFloatingSlot(Slot(440));
	pInventory.swapWithFloating(20);

	EXPECT_EQ(pInventory.getFloatingSlot()->isPresent(), false);
	EXPECT_EQ(pInventory.getSlotByIndex(20)->getItemId(), 440);
}

TEST(PlayerInventory, SelectedSlot) {
	mcp::inventory pInventory;

	pInventory.setSelectedIndex(4);
	EXPECT_EQ(pInventory.getSelectedIndex(), 4);
	EXPECT_EQ(pInventory.getSelectedIndex(true), 36 + 4);

	pInventory.setSelectedSlot(Slot(440));
	EXPECT_EQ(pInventory.getSelectedSlot()->getItemId(), 440);
}

TEST(PlayerInventory, SelectedSlot_WithWindow) {
	mcp::inventory pInventory;
	pInventory.openWindow(window::type::crafting);

	EXPECT_EQ(pInventory.getSelectedIndex(true), 36 + 1); //+1 is crafting offset

	pInventory.setSelectedIndex(4);
	EXPECT_EQ(pInventory.getSelectedIndex(), 4);
	EXPECT_EQ(pInventory.getSelectedIndex(true), 36 + 4 + 1);

	pInventory.setSelectedSlot(Slot(440));
	EXPECT_EQ(pInventory.getSelectedSlot()->getItemId(), 440);
}

TEST(PlayerInventory, OffhandSlot) {
	mcp::inventory pInventory;

	pInventory.setOffhandSlot(Slot(440));
	EXPECT_EQ(pInventory.getOffhandSlot()->getItemId(), 440);
}

TEST(PlayerInventory, OffhandSlot_WithWindow) {
	mcp::inventory pInventory;
	pInventory.openWindow(window::type::crafting);

	pInventory.setOffhandSlot(Slot(440));
	EXPECT_EQ(pInventory.getOffhandSlot()->getItemId(), 440);
}

TEST(PlayerInventory, HotbarSlot) {
	mcp::inventory pInventory;

	pInventory.setHotbarSlot(2, Slot(440));
	EXPECT_EQ(pInventory.getHotbarSlot(2)->getItemId(), 440);
}

TEST(PlayerInventory, HotbarSlot_WithWindow) {
	mcp::inventory pInventory;
	pInventory.openWindow(window::type::crafting);

	pInventory.setHotbarSlot(2, Slot(440));
	EXPECT_EQ(pInventory.getHotbarSlot(2)->getItemId(), 440);
}

TEST(PlayerInventory, AddAnywhere) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(36, Slot(440));

	Slot item(440);
	auto data = pInventory.addAnywhere(item);

	EXPECT_EQ(data.first, 1);
	EXPECT_EQ(data.second, 36);

	item = Slot(441);
	data = pInventory.addAnywhere(item);

	EXPECT_EQ(data.first, 1);
	EXPECT_EQ(data.second, 37);
}
TEST(PlayerInventory, AddToInventory) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(10, Slot(1062, 9));
	pInventory.setSlotByIndex(11, Slot(1062, 15));
	pInventory.setSlotByIndex(13, Slot(1062, 15));

	Slot item(1062, 16); //honey bottle, stackable up to 16

	auto data = pInventory.addToInventory(item);
	EXPECT_EQ(data.size(), 4);

	EXPECT_EQ(data[0].first, 7);
	EXPECT_EQ(data[0].second, 10);
	EXPECT_EQ(data[1].first, 1);
	EXPECT_EQ(data[1].second, 11);
	EXPECT_EQ(data[2].first, 1);
	EXPECT_EQ(data[2].second, 13);
	EXPECT_EQ(data[3].first, 7);
	EXPECT_EQ(data[3].second, 36);

	EXPECT_EQ(pInventory.getSlotByIndex(10)->count, 16);
	EXPECT_EQ(pInventory.getSlotByIndex(11)->count, 16);
	EXPECT_EQ(pInventory.getSlotByIndex(36)->count, 7);
	EXPECT_EQ(pInventory.getSlotByIndex(13)->count, 16);
}
TEST(PlayerInventory, AddToSlot) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(0, Slot(440));

	Slot item(440);
	Byte picked = pInventory.addToSlot(item, 0);

	EXPECT_EQ(picked, 1);
	EXPECT_EQ(pInventory.getSlotByIndex(0)->count, 2);

	item.count = 64;
	picked = pInventory.addToSlot(item, 0);

	EXPECT_EQ(picked, 62);
	EXPECT_EQ(pInventory.getSlotByIndex(0)->count, 64);
}

TEST(PlayerInventory, SwapSlot) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(30, Slot(440));
	pInventory.setSlotByIndex(40, Slot(420));

	pInventory.swapSlots(30, 40);

	EXPECT_EQ(pInventory.getSlotByIndex(30)->getItemId(), 420);
	EXPECT_EQ(pInventory.getSlotByIndex(40)->getItemId(), 440);
}
TEST(PlayerInventory, SwapSlot_WithWindow) {
	mcp::inventory pInventory;
	pInventory.openWindow(window::type::crafting);

	pInventory.setSlotByIndex(3, Slot(440));
	pInventory.setSlotByIndex(40, Slot(420));

	pInventory.swapSlots(3, 40);

	EXPECT_EQ(pInventory.getSlotByIndex(3)->getItemId(), 420);
	EXPECT_EQ(pInventory.getSlotByIndex(40)->getItemId(), 440);
}

TEST(PlayerInventory, StackItem) {
	mcp::inventory pInventory;

	pInventory.setSlotByIndex(30, Slot(440));

	Slot item(440);
	pInventory.stackItem(&item);

	EXPECT_EQ(item.count, 2);
	EXPECT_EQ(pInventory.getSlotByIndex(30)->isPresent(), false);
}
TEST(PlayerInventory, StackItem_WithWindow) {
	mcp::inventory pInventory;
	pInventory.openWindow(window::type::crafting);

	pInventory.setSlotByIndex(5, Slot(440));
	pInventory.setSlotByIndex(36, Slot(440));

	Slot item(440);
	pInventory.stackItem(&item);

	EXPECT_EQ(item.count, 3);
	EXPECT_EQ(pInventory.getSlotByIndex(36)->isPresent(), false);
	EXPECT_EQ(pInventory.getSlotByIndex(5)->isPresent(), false);
}