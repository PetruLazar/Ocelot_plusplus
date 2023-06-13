#pragma once

#include "../../block.h"
#include "../properties/distance.h"

namespace Blocks
{
	class LeafPersistentProperty : virtual public BlockState
	{
	public:
		LeafPersistentProperty(bool persistent) : value(persistent) {}
		virtual const LeafPersistentProperty* const persistent() const override { return this; }
		bool get() const { return value; }

	protected:
		bool value;
	};

	class Leaves : public DistanceProperty, public LeafPersistentProperty
	{
	protected:
		static bool placeLeaf(BlockEventArgs &args, int base_id)
		{
			return false;
		}

	public:
		Leaves(Byte distance, bool persistent) : DistanceProperty(distance), LeafPersistentProperty(persistent) {}

		virtual Transparency getTransparency(BlockFace) const override { return Transparency::partial; }

		virtual void free() const override { delete this; }
	};

	class Oak_Leaves : public Leaves
	{
		static constexpr int base_id = 148;

	public:
		Oak_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Spruce_Leaves : public Leaves
	{
		static constexpr int base_id = 162;

	public:
		Spruce_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Birch_Leaves : public Leaves
	{
		static constexpr int base_id = 176;

	public:
		Birch_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Jungle_Leaves : public Leaves
	{
		static constexpr int base_id = 190;

	public:
		Jungle_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Acacia_Leaves : public Leaves
	{
		static constexpr int base_id = 204;

	public:
		Acacia_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Dark_Oak_Leaves : public Leaves
	{
		static constexpr int base_id = 218;

	public:
		Dark_Oak_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Azalea_Leaves : public Leaves
	{
		static constexpr int base_id = 232;

	public:
		Azalea_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Flowering_Azalea_Leaves : public Leaves
	{
		static constexpr int base_id = 246;

	public:
		Flowering_Azalea_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		INHERIT_BLOCKPLACE(placeLeaf)
			static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
}