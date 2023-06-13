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
		virtual Transparency getTransparency(BlockFace) const override { return Transparency::partial; }
		virtual void free() const override { delete this; }

	protected:
		static bool placeLiquid(BlockEventArgs &args, int base_id);
	};

	class Water : public Liquid
	{
		static constexpr int base_id = 34;

	public:
		Water(Byte level) : Liquid(level) {}
		virtual int set(Byte level) const override { return getId(level); }

		INHERIT_BLOCKPLACE(placeLiquid)
			static int getId(Byte level) { return base_id + level; }
	};

	class Lava : public Liquid
	{
		static constexpr int base_id = 50;

	public:
		Lava(Byte level) : Liquid(level) {}
		//virtual int LevelProperty::set(Byte level) const override { return getId(level); }
		virtual int set(Byte level) const override { return getId(level); }

		INHERIT_BLOCKPLACE(placeLiquid)
			static int getId(Byte level) { return base_id + level; }
	};
}