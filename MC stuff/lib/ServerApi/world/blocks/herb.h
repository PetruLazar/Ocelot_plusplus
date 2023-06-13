#pragma once

#include "../block.h"

namespace Blocks
{
	class Herb : public BlockState
	{
	public:
		virtual bool replaceable() const override { return true; }

		virtual bool updateRoot(World* wld, int x, int y, int z) const override;

		virtual void free() const override { delete this; }

	protected:
		static bool placeHerb(BlockEventArgs &args, int base_id);
	};

	class Grass : public Herb
	{
		static constexpr int base_id = 1398;

	public:
		INHERIT_BLOCKPLACE(placeHerb)
		static int getId() { return base_id; }
	};
	class Fern : public Herb
	{
		static constexpr int base_id = 1399;

	public:
		INHERIT_BLOCKPLACE(placeHerb)
		static int getId() { return base_id; }
	};
}