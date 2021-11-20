#pragma once

namespace window
{
	static unsigned indexer = 0;

	inline unsigned getID() { return indexer++; }

	enum type {
		generic_9x1 = 0,
		generic_9x2,
		generic_9x3,
		generic_9x4,
		generic_9x5,
		generic_9x6,
		generic_3x3,
		anvil,
		beacon,
		blast_furnace,
		brewing_stand,
		crafting,
		enchantment,
		furnace,
		grindstone,
		hopper,
		lectern,
		loom,
		merchant,
		shulker_box,
		smithing,
		smoker,
		cartography,
		stonecutter
	};
}


