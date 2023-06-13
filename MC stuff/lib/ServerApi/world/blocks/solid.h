#pragma once

#include "properties/snowy.h"
#include "properties/powered.h"
#include "properties/note.h"
#include "properties/instrument.h"

namespace Blocks
{
	class SolidBlock : virtual public BlockState
	{
	protected:
		static bool placeSolid(BlockEventArgs &args, int base_id);

	public:
		virtual void free() const override { delete this; }

		virtual Transparency getTransparency(BlockFace) const override { return Transparency::solid; }
	};

	class SnowyBlock : virtual public SolidBlock
	{
	public:
		SnowyBlock(bool snowy) : value(snowy) {}
		virtual const SnowyBlock* const snowy() const override { return this; }
		bool get() const { return value; }

		virtual void free() const override { delete this; }

	protected:
		static bool placeSnowy(BlockEventArgs &args, int base_id);

		bool value;
	};

	class Air : public BlockState
	{
		static constexpr int base_id = 0;
	public:
		virtual bool replaceable() const override { return true; }

		static bool place(BlockEventArgs &args) { return false; }
		static int getId() { return base_id; }

		virtual void free() const override {delete this;}
	};
	class Stone : public SolidBlock
	{
		static constexpr int base_id = 1;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Granite : public SolidBlock
	{
		static constexpr int base_id = 2;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Polished_Granite : public SolidBlock
	{
		static constexpr int base_id = 3;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Diorite : public SolidBlock
	{
		static constexpr int base_id = 4;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Polished_Diorite : public SolidBlock
	{
		static constexpr int base_id = 5;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Andesite : public SolidBlock
	{
		static constexpr int base_id = 6;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Polished_Andesite : public SolidBlock
	{
		static constexpr int base_id = 7;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Grass_Block : public SnowyBlock
	{
		static constexpr int base_id = 8;

	public:
		Grass_Block(bool snowy) : SnowyBlock(snowy) {}

		INHERIT_BLOCKPLACE(placeSnowy)
			virtual bool randomTick(World* wld, int x, int y, int z) const override;
		virtual bool updateRoot(World* wld, int x, int y, int z) const override;

		static int getId(bool snowy = false) { return base_id + !snowy; }
	};
	class Dirt : public SolidBlock
	{
		static constexpr int base_id = 10;

	public:
		INHERIT_BLOCKPLACE(placeSolid)

			static int getId() { return base_id; }
	};
	class Coarse_Dirt : public SolidBlock
	{
		static constexpr int base_id = 11;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Podzol : public SnowyBlock
	{
		static constexpr int base_id = 12;

	public:
		Podzol(bool snowy) : SnowyBlock(snowy) {}

		INHERIT_BLOCKPLACE(placeSnowy)
			static int getId(bool snowy = false) { return base_id + !snowy; }
	};
	class Cobblestone : public SolidBlock
	{
		static constexpr int base_id = 14;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};

	class Oak_Planks : public SolidBlock
	{
		static constexpr int base_id = 15;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Spruce_Planks : public SolidBlock
	{
		static constexpr int base_id = 16;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Birch_Planks : public SolidBlock
	{
		static constexpr int base_id = 17;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Jungle_Planks : public SolidBlock
	{
		static constexpr int base_id = 18;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Acacia_Planks : public SolidBlock
	{
		static constexpr int base_id = 19;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Dark_Oak_Planks : public SolidBlock
	{
		static constexpr int base_id = 20;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};

	class Bedrock : public SolidBlock
	{
		static constexpr int base_id = 33;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};

	class Sand : public SolidBlock
	{
		static constexpr int base_id = 66;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Red_Sand : public SolidBlock
	{
		static constexpr int base_id = 67;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Gravel : public SolidBlock
	{
		static constexpr int base_id = 68;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Sandstone : public SolidBlock
	{
		static constexpr int base_id = 68;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Chiseled_Sandstone : public SolidBlock
	{
		static constexpr int base_id = 68;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Cut_Sandstone : public SolidBlock
	{
		static constexpr int base_id = 68;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	
	class Gold_Ore : public SolidBlock
	{
		static constexpr int base_id = 69;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Deepslate_Gold_Ore : public SolidBlock
	{
		static constexpr int base_id = 70;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Iron_Ore : public SolidBlock
	{
		static constexpr int base_id = 71;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Deepslate_Iron_Ore : public SolidBlock
	{
		static constexpr int base_id = 72;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Coal_Ore : public SolidBlock
	{
		static constexpr int base_id = 73;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Deepslate_Coal_Ore : public SolidBlock
	{
		static constexpr int base_id = 74;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Lapis_Ore : public SolidBlock
	{
		static constexpr int base_id = 263;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Deepslate_Lapis_Ore : public SolidBlock
	{
		static constexpr int base_id = 264;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Nether_Gold_Ore : public SolidBlock
	{
		static constexpr int base_id = 75;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};

	class Lapis_Block : public SolidBlock
	{
		static constexpr int base_id = 265;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};

	class Sponge : public SolidBlock
	{
		static constexpr int base_id = 260;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Wet_Sponge : public SolidBlock
	{
		static constexpr int base_id = 261;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};
	class Glass : public SolidBlock
	{
		static constexpr int base_id = 262;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};

	class Dispenser : public SolidBlock, public FacingProperty, public TriggeredProperty
	{
		static constexpr int base_id = 266;

	public:
		Dispenser(Facing facing = Facing::north, bool triggered = false) : FacingProperty(facing), TriggeredProperty(triggered) {}

		bool rightClick(BlockEventArgs &args) const override;

		static bool place(BlockEventArgs &args);
		static int getId(Facing facing = Facing::north, bool triggered = false) { return base_id + !triggered + ((int)facing << 1); }

		virtual void free() const override { delete this; }
	};

	class Note_Block : public SolidBlock, public PoweredProperty, public NoteProperty, public InstrumentProperty
	{
		static constexpr int base_id = 281;

	public:
		Note_Block(Instrument instrument = harp, Byte note = 0, bool powered = false) : InstrumentProperty(instrument), NoteProperty(note), PoweredProperty(powered) {}

		bool rightClick(BlockEventArgs &args) const override;

		static bool place(BlockEventArgs &args);
		static int getId(Instrument instrument = harp, Byte note = 0, bool powered = false) { return base_id + !powered + (note << 1) + instrument * 50; }

		//maybe inherit from SolidBlock?
		//virtual Transparency getTransparency(BlockFace) const override { return Transparency::solid; }

		virtual void free() const override { delete this; }
	};
}