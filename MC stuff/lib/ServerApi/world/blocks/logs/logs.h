#pragma once

#include "../../block.h"

namespace Blocks
{
	class Log : public AxisProperty
	{
	public:
		Log(AxisOrientation axis) : AxisProperty(axis) {}

		virtual void free() const override { delete this; }

	protected:
		static bool placeLog(DECL_BLOCKPLACE_ARGS, int base_id);

		bool strip(World* wld, int x, int y, int z, Item heldItem, int strippedId) const;

		//maybe derive from SolidBlock?
		virtual Transparency getTransparency(BlockFace) const override { return Transparency::solid; }
	};

	class Stripped_Oak_Log : public Log
	{
		static constexpr int base_id = 109;

	public:
		Stripped_Oak_Log(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Spruce_Log : public Log
	{
		static constexpr int base_id = 94;

	public:
		Stripped_Spruce_Log(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Birch_Log : public Log
	{
		static constexpr int base_id = 97;

	public:
		Stripped_Birch_Log(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};

	class Oak_Log : public Log
	{
		static constexpr int base_id = 76;

	public:
		Oak_Log(AxisProperty::AxisOrientation axis) : Log(axis) {}

		virtual bool rightClick(World* wld, int x, int y, int z, Item heldItem) const override { return strip(wld, x, y, z, heldItem, Stripped_Oak_Log::getId(AxisProperty::value)); }

		INHERIT_BLOCKPLACE(placeLog)
		static int getId(AxisProperty::AxisOrientation axis) { return base_id + axis; }
	};
	class Spruce_Log : public Log
	{
		static constexpr int base_id = 79;

	public:
		Spruce_Log(AxisOrientation axis) : Log(axis) {}

		virtual bool rightClick(World* wld, int x, int y, int z, Item heldItem) const override { return strip(wld, x, y, z, heldItem, Stripped_Spruce_Log::getId(AxisProperty::value)); }

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Birch_Log : public Log
	{
		static constexpr int base_id = 82;

	public:
		Birch_Log(AxisOrientation axis) : Log(axis) {}

		virtual bool rightClick(World* wld, int x, int y, int z, Item heldItem) const override { return strip(wld, x, y, z, heldItem, Stripped_Birch_Log::getId(AxisProperty::value)); }

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
}