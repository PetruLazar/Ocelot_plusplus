#pragma once

#include "properties/snowy.h"
#include "properties/powered.h"
#include "properties/note.h"
#include "properties/instrument.h"

namespace Blocks
{
	class SolidBlock : public BlockState
	{
	protected:
		static bool placeSolid(DECL_BLOCKPLACE_ARGS, int base_id);

	public:
		virtual void free() const override { delete this; }

		virtual Transparency getTransparency(BlockFace) const override { return Transparency::solid; }
	};

	class Air : public SolidBlock
	{
		static constexpr int base_id = 0;
	public:
		virtual bool replaceable() const override { return true; }		

		static bool place(DECL_BLOCKPLACE_ARGS) { return false; }
		static int getId() { return base_id; }
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
	class Bedrock : public SolidBlock
	{
		static constexpr int base_id = 33;

	public:
		INHERIT_BLOCKPLACE(placeSolid)
			static int getId() { return base_id; }
	};

	class Note_Block : public PoweredProperty, public NoteProperty, public InstrumentProperty
	{
		static constexpr int base_id = 281;

	public:
		Note_Block(Instrument instrument = harp, Byte note = 0, bool powered = false) : InstrumentProperty(instrument), NoteProperty(note), PoweredProperty(powered) {}

		bool rightClick(World* wld, int x, int y, int z, Item heldItem) const override;

		static bool place(DECL_BLOCKPLACE_ARGS);
		static int getId(Instrument instrument = harp, Byte note = 0, bool powered = false) { return base_id + !powered + (note << 1) + instrument * 50; }

		//maybe inherit from SolidBlock?
		virtual Transparency getTransparency(BlockFace) const override { return Transparency::solid; }

		virtual void free() const override { delete this; }
	};
}