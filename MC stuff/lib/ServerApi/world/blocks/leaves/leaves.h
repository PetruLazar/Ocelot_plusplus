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
		static bool placeLeaf(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId, int base_id)
		{
			return false;
		}

	public:
		Leaves(Byte distance, bool persistent) : DistanceProperty(distance), LeafPersistentProperty(persistent) {}

		virtual void free() const override { delete this; }
	};

	class Oak_Leaves : public Leaves
	{
		static constexpr int base_id = 148;

	public:
		Oak_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId) { return placeLeaf(wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId, base_id); }
		static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Spruce_Leaves : public Leaves
	{
		static constexpr int base_id = 162;

	public:
		Spruce_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId) { return placeLeaf(wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId, base_id); }
		static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
	class Birch_Leaves : public Leaves
	{
		static constexpr int base_id = 176;

	public:
		Birch_Leaves(Byte distance, bool persistent) : Leaves(distance, persistent) {}

		static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId) { return placeLeaf(wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId, base_id); }
		static int getId(Byte distance, bool persistent) { return base_id + (distance - 1) * 2 + !persistent; }
	};
}