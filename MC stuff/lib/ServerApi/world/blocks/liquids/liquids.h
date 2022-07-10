#pragma once

#include "../../block.h"

namespace Blocks
{
	class Liquid : public Blocks::LevelProperty
	{
	public:
		Liquid(Byte level) : LevelProperty(level) {}

		virtual bool replaceable() const override { return true; }
		virtual bool isLiquid() const override { return true; }
		virtual void free() const override { delete this; }

	protected:
		static bool placeLiquid(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId, int base_id);
	};

	class Water : public Liquid
	{
		static constexpr int base_id = 34;

	public:
		Water(Byte level) : Liquid(level) {}
		virtual int set(Byte level) const override { return getId(level); }

		static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId) { return placeLiquid(wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId, base_id); }
		static int getId(Byte level) { return base_id + level; }
	};

	class Lava : public Liquid
	{
		static constexpr int base_id = 50;

	public:
		Lava(Byte level) : Liquid(level) {}
		//virtual int LevelProperty::set(Byte level) const override { return getId(level); }
		virtual int set(Byte level) const override { return getId(level); }

		static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId) { return placeLiquid(wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId, base_id); }
		static int getId(Byte level) { return base_id + level; }
	};
}