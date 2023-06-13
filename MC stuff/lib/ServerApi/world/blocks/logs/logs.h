#pragma once

#include "../../block.h"

#define WOOD_STRIP_DEFINE(target) virtual bool rightClick(BlockEventArgs &args) const override { return strip(args, target::getId(AxisProperty::value)); }

namespace Blocks
{
	class Log : public SolidBlock, public AxisProperty
	{
	public:
		Log(AxisOrientation axis) : AxisProperty(axis) {}

		virtual void free() const override { delete this; }

	protected:
		static bool placeLog(BlockEventArgs &args, int base_id);

		bool strip(BlockEventArgs &args, int strippedId) const;
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
	class Stripped_Jungle_Log : public Log
	{
		static constexpr int base_id = 100;

	public:
		Stripped_Jungle_Log(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Acacia_Log : public Log
	{
		static constexpr int base_id = 103;

	public:
		Stripped_Acacia_Log(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Dark_Oak_Log : public Log
	{
		static constexpr int base_id = 106;

	public:
		Stripped_Dark_Oak_Log(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Oak_Log : public Log
	{
		static constexpr int base_id = 109;

	public:
		Stripped_Oak_Log(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};

	class Oak_Log : public Log
	{
		static constexpr int base_id = 76;

	public:
		Oak_Log(AxisProperty::AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Oak_Log)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisProperty::AxisOrientation axis) { return base_id + axis; }
	};
	class Spruce_Log : public Log
	{
		static constexpr int base_id = 79;

	public:
		Spruce_Log(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Spruce_Log)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Birch_Log : public Log
	{
		static constexpr int base_id = 82;

	public:
		Birch_Log(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Birch_Log)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Jungle_Log : public Log
	{
		static constexpr int base_id = 85;

	public:
		Jungle_Log(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Jungle_Log)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Acacia_Log : public Log
	{
		static constexpr int base_id = 88;

	public:
		Acacia_Log(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Acacia_Log)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Dark_Oak_Log : public Log
	{
		static constexpr int base_id = 91;

	public:
		Dark_Oak_Log(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Dark_Oak_Log)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};

	class Stripped_Oak_Wood : public Log
	{
		static constexpr int base_id = 130;

	public:
		Stripped_Oak_Wood(AxisProperty::AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisProperty::AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Spruce_Wood : public Log
	{
		static constexpr int base_id = 133;

	public:
		Stripped_Spruce_Wood(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Birch_Wood : public Log
	{
		static constexpr int base_id = 136;

	public:
		Stripped_Birch_Wood(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Jungle_Wood : public Log
	{
		static constexpr int base_id = 139;

	public:
		Stripped_Jungle_Wood(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Acacia_Wood : public Log
	{
		static constexpr int base_id = 142;

	public:
		Stripped_Acacia_Wood(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Stripped_Dark_Oak_Wood : public Log
	{
		static constexpr int base_id = 145;

	public:
		Stripped_Dark_Oak_Wood(AxisOrientation axis) : Log(axis) {}

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};

	class Oak_Wood : public Log
	{
		static constexpr int base_id = 112;

	public:
		Oak_Wood(AxisProperty::AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Oak_Wood)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisProperty::AxisOrientation axis) { return base_id + axis; }
	};
	class Spruce_Wood : public Log
	{
		static constexpr int base_id = 115;

	public:
		Spruce_Wood(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Spruce_Wood)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Birch_Wood : public Log
	{
		static constexpr int base_id = 118;

	public:
		Birch_Wood(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Birch_Wood)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Jungle_Wood : public Log
	{
		static constexpr int base_id = 121;

	public:
		Jungle_Wood(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Jungle_Wood)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Acacia_Wood : public Log
	{
		static constexpr int base_id = 124;

	public:
		Acacia_Wood(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Acacia_Wood)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
	class Dark_Oak_Wood : public Log
	{
		static constexpr int base_id = 127;

	public:
		Dark_Oak_Wood(AxisOrientation axis) : Log(axis) {}

		WOOD_STRIP_DEFINE(Stripped_Dark_Oak_Wood)

		INHERIT_BLOCKPLACE(placeLog)
			static int getId(AxisOrientation axis) { return base_id + axis; }
	};
}