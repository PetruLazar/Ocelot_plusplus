
#include "window.h"

unsigned window::getWindowSlotCount(window::type windowType)
{
	switch (windowType)
	{
	case window::type::generic_9x1:
	case window::type::generic_3x3:
		return 9;
	case window::type::generic_9x2:
		return 18;
	case window::type::shulker_box:
	case window::type::generic_9x3:
		return 27;
	case window::type::generic_9x4:
		return 36;
	case window::type::generic_9x5:
		return 45;
	case window::type::generic_9x6:
		return 54;
	case window::type::beacon:
	case window::type::lectern:
		return 1;
	case window::type::anvil:
	case window::type::smoker:
	case window::type::furnace:
	case window::type::blast_furnace:
	case window::type::cartography:
	case window::type::grindstone:
	case window::type::smithing:
	case window::type::merchant:
		return 3;
	case window::type::brewing_stand:
	case window::type::hopper:
		return 5;
	case window::type::crafting:
		return 10;
	case window::type::loom:
		return 4;
	case window::type::enchantment:
	case window::type::stonecutter:
		return 2;
	}

	return -1; //should never get here
}