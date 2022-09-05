#include "../world.h"
#include "../types/position.h"
#include "../debug/mcexceptions.h"

const BlockState* const BlockState::globalPalette[] =
{
	/*   0   */ new Blocks::Air(),
	/*   1   */ new Blocks::Stone(),
	/*   2   */ new Blocks::Granite(),
	/*   3   */ new Blocks::Polished_Granite(),
	/*   4   */ new Blocks::Diorite(),
	/*   5   */ new Blocks::Polished_Diorite(),
	/*   6   */ new Blocks::Andesite(),
	/*   7   */ new Blocks::Polished_Andesite(),
	/*   8   */ new Blocks::Grass_Block(true),
	/*   9   */ new Blocks::Grass_Block(false),
	/*  10   */ new Blocks::Dirt(),
	/*  11   */ new Blocks::Coarse_Dirt(),
	/*  12   */ new Blocks::Podzol(true),
	/*  13   */ new Blocks::Podzol(false),
	/*  14   */ new Blocks::Cobblestone(),
	/*15 - 24*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*25 - 32*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*  33   */ new Blocks::Bedrock(),
	/*34 - 49*/ new Blocks::Water(0), new Blocks::Water(1), new Blocks::Water(2), new Blocks::Water(3), new Blocks::Water(4), new Blocks::Water(5), new Blocks::Water(6), new Blocks::Water(7), new Blocks::Water(8), new Blocks::Water(9), new Blocks::Water(10), new Blocks::Water(11), new Blocks::Water(12), new Blocks::Water(13), new Blocks::Water(14), new Blocks::Water(15),
	/*50 - 65*/ new Blocks::Lava(0), new Blocks::Lava(1), new Blocks::Lava(2), new Blocks::Lava(3), new Blocks::Lava(4), new Blocks::Lava(5), new Blocks::Lava(6), new Blocks::Lava(7), new Blocks::Lava(8), new Blocks::Lava(9), new Blocks::Lava(10), new Blocks::Lava(11), new Blocks::Lava(12), new Blocks::Lava(13), new Blocks::Lava(14), new Blocks::Lava(15),
	/*66 - 75*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

	/*76 - 78*/ new Blocks::Oak_Log(Blocks::AxisProperty::AxisOrientation::X), new Blocks::Oak_Log(Blocks::AxisProperty::AxisOrientation::Y), new Blocks::Oak_Log(Blocks::AxisProperty::AxisOrientation::Z),
	/*79 - 81*/ new Blocks::Spruce_Log(Blocks::AxisProperty::AxisOrientation::X), new Blocks::Spruce_Log(Blocks::AxisProperty::AxisOrientation::Y), new Blocks::Spruce_Log(Blocks::AxisProperty::AxisOrientation::Z),
	/*82 - 84*/ new Blocks::Birch_Log(Blocks::AxisProperty::AxisOrientation::X), new Blocks::Birch_Log(Blocks::AxisProperty::AxisOrientation::Y), new Blocks::Birch_Log(Blocks::AxisProperty::AxisOrientation::Z),
	/*85 - 87*/ nullptr, nullptr, nullptr,
	/*88 - 90*/ nullptr, nullptr, nullptr,
	/*91 - 93*/ nullptr, nullptr, nullptr,
	/*94 - 96*/ new Blocks::Stripped_Spruce_Log(Blocks::AxisProperty::AxisOrientation::X), new Blocks::Stripped_Spruce_Log(Blocks::AxisProperty::AxisOrientation::Y), new Blocks::Stripped_Spruce_Log(Blocks::AxisProperty::AxisOrientation::Z),
	/*97 - 99*/ new Blocks::Stripped_Birch_Log(Blocks::AxisProperty::AxisOrientation::X), new Blocks::Stripped_Birch_Log(Blocks::AxisProperty::AxisOrientation::Y), new Blocks::Stripped_Birch_Log(Blocks::AxisProperty::AxisOrientation::Z),
	/*100-108*/ nullptr, nullptr, nullptr,
	/*103-105*/ nullptr, nullptr, nullptr,
	/*106-108*/ nullptr, nullptr, nullptr,
	/*109-111*/ new Blocks::Stripped_Oak_Log(Blocks::AxisProperty::AxisOrientation::X), new Blocks::Stripped_Oak_Log(Blocks::AxisProperty::AxisOrientation::Y), new Blocks::Stripped_Oak_Log(Blocks::AxisProperty::AxisOrientation::Z),
	/*112-114*/ nullptr, nullptr, nullptr,
	/*115-124*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*125-134*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*135-144*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*145-147*/ nullptr, nullptr, nullptr,

	/*148-149*/ new Blocks::Oak_Leaves(1, true), new Blocks::Oak_Leaves(1, false),
	/*150-151*/ new Blocks::Oak_Leaves(2, true), new Blocks::Oak_Leaves(2, false),
	/*152-153*/ new Blocks::Oak_Leaves(3, true), new Blocks::Oak_Leaves(3, false),
	/*154-155*/ new Blocks::Oak_Leaves(4, true), new Blocks::Oak_Leaves(4, false),
	/*156-157*/ new Blocks::Oak_Leaves(5, true), new Blocks::Oak_Leaves(5, false),
	/*158-159*/ new Blocks::Oak_Leaves(6, true), new Blocks::Oak_Leaves(6, false),
	/*160-161*/ new Blocks::Oak_Leaves(7, true), new Blocks::Oak_Leaves(7, false),
	/*162-163*/ new Blocks::Spruce_Leaves(1, true), new Blocks::Spruce_Leaves(1, false),
	/*164-165*/ new Blocks::Spruce_Leaves(2, true), new Blocks::Spruce_Leaves(2, false),
	/*166-167*/ new Blocks::Spruce_Leaves(3, true), new Blocks::Spruce_Leaves(3, false),
	/*168-169*/ new Blocks::Spruce_Leaves(4, true), new Blocks::Spruce_Leaves(4, false),
	/*170-171*/ new Blocks::Spruce_Leaves(5, true), new Blocks::Spruce_Leaves(5, false),
	/*172-173*/ new Blocks::Spruce_Leaves(6, true), new Blocks::Spruce_Leaves(6, false),
	/*174-175*/ new Blocks::Spruce_Leaves(7, true), new Blocks::Spruce_Leaves(7, false),
	/*176-177*/ new Blocks::Birch_Leaves(1, true), new Blocks::Birch_Leaves(1, false),
	/*178-179*/ new Blocks::Birch_Leaves(2, true), new Blocks::Birch_Leaves(2, false),
	/*180-181*/ new Blocks::Birch_Leaves(3, true), new Blocks::Birch_Leaves(3, false),
	/*182-183*/ new Blocks::Birch_Leaves(4, true), new Blocks::Birch_Leaves(4, false),
	/*184-185*/ new Blocks::Birch_Leaves(5, true), new Blocks::Birch_Leaves(5, false),
	/*186-187*/ new Blocks::Birch_Leaves(6, true), new Blocks::Birch_Leaves(6, false),
	/*188-189*/ new Blocks::Birch_Leaves(7, true), new Blocks::Birch_Leaves(7, false),
	/*  190  */ nullptr,
	/*191-200*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*201-280*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

	/*281-1080*/ new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::harp, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::basedrum, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::snare, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::hat, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bass, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::flute, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bell, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::guitar, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::chime, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::xylophone, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::iron_xylophone, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::cow_bell, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::didgeridoo, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::bit, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::banjo, 24, false),

	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 0, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 0, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 1, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 1, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 2, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 2, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 3, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 3, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 4, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 4, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 5, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 5, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 6, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 6, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 7, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 7, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 8, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 8, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 9, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 9, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 10, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 10, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 11, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 11, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 12, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 12, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 13, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 13, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 14, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 14, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 15, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 15, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 16, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 16, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 17, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 17, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 18, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 18, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 19, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 19, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 20, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 20, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 21, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 21, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 22, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 22, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 23, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 23, false),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 24, true),
	new Blocks::Note_Block(Blocks::InstrumentProperty::pling, 24, false),

	/*1081-1100*/nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1101-1200*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1201-1300*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1301-1310*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1311-1320*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1321-1330*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1331-1340*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1341-1350*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1351-1360*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1361-1370*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1371-1380*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1381-1390*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	/*1391-1397*/ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	new Blocks::Grass(),
	new Blocks::Fern(),
};

/*BlockState::BlockState(int id) : id(id), state(&Registry::getBlockState(id)) {}
BlockState::BlockState(json* blockState) : state(blockState), id((*blockState)["id"].iValue()) {}
BlockState::BlockState(json& blockState) : state(&blockState), id(blockState["id"].iValue()) {}
BlockState::BlockState(const std::string blockName) : state(&Registry::getBlockState(blockName))
{
	id = (*state)["id"].iValue();
}
BlockState::BlockState(const std::string blockName, BlockProperty* properties) : state(&Registry::getBlockState(blockName, properties))
{
	id = (*state)["id"].iValue();
}

std::string BlockState::getState(const std::string& stateName) const
{
	json& stateProp = (*state)["properties"][stateName];
	return stateProp.getType() == json::type::string ? stateProp.value() : stateProp.stringValue();
}
json* BlockState::getJsonState() const
{
	return state;
}
void BlockState::setState(const std::string& stateName, const std::string& value)
{
	json& stateProps = (*state)["properties"];
	ull propCount = stateProps.getSize();
	BlockProperty* desiredProps = new BlockProperty[propCount];
	for (ull i = 0; i < propCount; i++)
	{
		json& prop = stateProps[(int)i];
		BlockProperty& desiredProp = desiredProps[i];
		desiredProp.name = prop.getName();
		if (desiredProp.name == stateName) desiredProp.value = value;
		else desiredProp.value = prop.getType() == json::type::string ? prop.value() : prop.stringValue();
	}
	state = &Registry::getBlockState(Registry::getBlock(id), desiredProps);
	id = (*state)["id"].iValue();
	delete[] desiredProps;
}

void BlockState::set(int blockId)
{
	state = &Registry::getBlockState(blockId);
	id = (*state)["id"].iValue();
}
void BlockState::set(const std::string& blockName)
{
	state = &Registry::getBlockState(blockName);
	id = (*state)["id"].iValue();
}
void BlockState::set(const std::string& blockName, BlockProperty* properties)
{
	state = &Registry::getBlockState(blockName, properties);
	id = (*state)["id"].iValue();
}

bool BlockState::operator==(const BlockState& that) const
{
	return (int&)id == (int&)that.id;
}
void BlockState::operator=(const BlockState& that)
{
	id = that.id;
	state = that.state;
}*/

//PaletteEntry::PaletteEntry(int blockid, short refCount) : blockid(blockid), referenceCount(refCount) {}

BlockEntity::BlockEntity(Byte packedXZ, bshort y, varInt type, nbt_compound* tags) : packedXZ(packedXZ), y(y), type(type), tags(tags) {}
BlockEntity::~BlockEntity()
{
	delete tags;
}

void BlockEntity::writeExplicit(char*& buffer)
{
	Position((*tags)["x"].vInt(), (*tags)["y"].vInt(), (*tags)["z"].vInt()).write(buffer);
	type.write(buffer);
	tags->write(buffer);
}
void BlockEntity::write(char*& buffer)
{
	*(buffer++) = packedXZ;
	y.write(buffer);
	type.write(buffer);
	tags->write(buffer);
}
void BlockState::updateAround(World* wld, int x, int y, int z)
{
	//+y
	if (wld->checkCoordinatesUpper(y + 1)) globalPalette[wld->getBlock(x, y + 1, z)]->update(wld, x, y + 1, z);

	//-y
	if (wld->checkCoordinatesLower(y - 1)) globalPalette[wld->getBlock(x, y - 1, z)]->update(wld, x, y - 1, z);

	//+x
	int id = wld->getBlock(x + 1, y, z);
	if (id != -1) globalPalette[id]->update(wld, x + 1, y, z);

	//-x
	id = wld->getBlock(x - 1, y, z);
	if (id != -1) globalPalette[id]->update(wld, x - 1, y, z);

	//+z
	id = wld->getBlock(x, y, z + 1);
	if (id != -1) globalPalette[id]->update(wld, x, y, z + 1);

	//-z
	id = wld->getBlock(x, y, z - 1);
	if (id != -1) globalPalette[id]->update(wld, x, y, z - 1);
}

namespace Blocks
{
	bool SolidBlock::placeSolid(DECL_BLOCKPLACE_ARGS, int base_id)
	{
		const BlockState* currentBlock = BlockState::globalPalette[currentBlockId];
		if (currentBlock->replaceable())
		{
			wld->setBlock(x, y, z, base_id);
			return true;
		}
		return false;
	}

	bool SnowyBlock::placeSnowy(DECL_BLOCKPLACE_ARGS, int base_id)
	{
		const BlockState* currentBlock = BlockState::globalPalette[currentBlockId];
		if (currentBlock->replaceable() && currentBlockId != base_id)
		{
			wld->setBlock(x, y, z, base_id + 1 /*+1 = snowy: false*/);
			return true;
		}
		return false;
	}

	bool Herb::placeHerb(DECL_BLOCKPLACE_ARGS, int base_id)
	{
		const BlockState* currentBlock = BlockState::globalPalette[currentBlockId];
		if (currentBlock->replaceable() && currentBlockId != base_id)
		{
			wld->setBlock(x, y, z, base_id);
			return true;
		}
		return false;
	}
	bool Herb::updateRoot(World* wld, int x, int y, int z) const
	{
		if (--y < 0 || wld->getBlock(x, y, z) == Air::getId())
		{
			wld->setBlock(x, ++y, z, Air::getId());
			return true;
		}
		return false;
	}

	bool Liquid::placeLiquid(DECL_BLOCKPLACE_ARGS, int base_id)
	{
		const BlockState* currentBlock = BlockState::globalPalette[currentBlockId];
		if (currentBlock->isLiquid())
		{
			const LevelProperty* level = currentBlock->level();
			int id = level->set((level->get() + 1) % 16);
			wld->setBlock(x, y, z, id);
			return true;
		}
		if (currentBlock->replaceable())
		{
			wld->setBlock(x, y, z, base_id);
			return true;
		}
		return false;
	}

	bool Log::placeLog(DECL_BLOCKPLACE_ARGS, int base_id)
	{
		const BlockState* currentBlock = BlockState::globalPalette[currentBlockId];
		if (currentBlock->replaceable())
		{
			AxisOrientation axis = Y;
			switch (face)
			{
			case BlockFace::north:
			case BlockFace::south:
				axis = AxisOrientation::Z;
				break;
			case BlockFace::west:
			case BlockFace::east:
				axis = AxisOrientation::X;
			}
			wld->setBlock(x, y, z, base_id + axis);
			return true;
		}
		return false;
	}
	bool Log::strip(World* wld, int x, int y, int z, Item heldItem, int strippedId) const
	{
		switch (heldItem)
		{
		case Item::minecraft_wooden_axe:
		case Item::minecraft_stone_axe:
		case Item::minecraft_iron_axe:
		case Item::minecraft_golden_axe:
		case Item::minecraft_diamond_axe:
		case Item::minecraft_netherite_axe:
			wld->setBlock(x, y, z, strippedId);
			return true;
		}
		return false;
	}

	bool Note_Block::place(DECL_BLOCKPLACE_ARGS)
	{
		const BlockState* currentBlock = BlockState::globalPalette[currentBlockId];
		if (currentBlock->replaceable())
		{
			wld->setBlock(x, y, z, getId(Instrument(rand() & 15), rand() % 25, rand() & 1));
			return true;
		}
		return false;
	}
	bool Note_Block::rightClick(World* wld, int x, int y, int z, Item heldItem) const
	{
		Byte newNote = NoteProperty::value + 1;
		if (newNote == 25) newNote = 0;
		wld->setBlock(x, y, z, getId(InstrumentProperty::value, newNote, PoweredProperty::value));
		return false;
	}

	bool Grass_Block::randomTick(World* wld, int x, int y, int z) const
	{
		int action = rand() & 0xff;

		switch (action & 0b11)
		{
		default:
		{
			//expand
			int dx = action >> 2 & 0b1,
				dz = action >> 3 & 0b1,
				dy = action >> 4 & 0b11;

			if (!dx) dx--;
			if (!dz) dz--;
			switch (dy)
			{
			case 0b00:
			case 0b01:
				break;
			case 0b10:
				dy = -1;
				break;
			case 0b11:
				return false;
			}

			y += dy;
			if (!wld->checkCoordinates(y)) return false;

			x += dx;
			z += dz;
			if (wld->getBlock(x, y, z) != Dirt::getId()) return false;
			int topy = y + 1;
			if (wld->checkCoordinates(topy))
			{
				if (wld->getBlock(x, topy, z) != 0) return false; //do not expand grass if target block is covered
			}
			wld->setBlock(x, y, z, Grass_Block::getId(false));
			return true;
		}
		case 0x2:
			//decay
			y++;
			if (!wld->checkCoordinates(y)) return false;
			if (wld->getBlock(x, y, z) == 0) return false;
			wld->setBlock(x, y - 1, z, Dirt::getId());
			return true;
		}
		return false;
	}
	bool Grass_Block::updateRoot(World* wld, int x, int y, int z) const
	{
		if (!wld->checkCoordinatesUpper(++y)) return false;
		int topid = wld->getBlock(x, y, z);
		bool newSnowy = (topid == /*snow layer or block*/-1);
		if (newSnowy != SnowyBlock::value)
		{
			wld->setBlock(x, y, z, getId(newSnowy));
			return true;
		}
		return false;
	}
}