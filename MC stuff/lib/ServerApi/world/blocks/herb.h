#pragma once

#include "../block.h"

namespace Blocks
{
	class Herb : public BlockState
	{
	public:
		virtual bool replaceable() const override { return true; }

		virtual void free() const override { delete this; }

	protected:
		static bool placeHerb(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId, int base_id);
	};

	class Grass : public Herb
	{
		static constexpr int base_id = 1398;

	public:
		static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId) { return placeHerb(wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId, base_id); }
		static int getId() { return base_id; }
	};
	class Fern : public Herb
	{
		static constexpr int base_id = 1399;

	public:
		static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId) { return placeHerb(wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId, base_id); }
		static int getId() { return base_id; }
	};
}