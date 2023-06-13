#pragma once

#include "../block.h"

#define UPDATE_BED() virtual bool updateRoot(World* wld, int x, int y, int z) const override { \
	return updateBed(wld, x, y, z, getId((Facing2D)FacingProperty::value, OccupiedProperty::value, Part(-PartProperty::value + 1))); }

namespace Blocks
{
	class Bed : public FacingProperty, public OccupiedProperty, public PartProperty
	{
	public:
		Bed(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) : FacingProperty((Facing)facing), PartProperty(part), OccupiedProperty(occupied) {}
		virtual void free() const override { delete this; }

		bool updateBed(World* wld, int x, int y, int z, int expectId) const;

	protected:
		static bool placeBed(BlockEventArgs& args, int base_id);

		bool sleep(BlockEventArgs &args) const;
	};

	class White_Bed : public Bed
	{
		static constexpr int base_id = 1081;

	public:
		White_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

			INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Orange_Bed : public Bed
	{
		static constexpr int base_id = 1097;

	public:
		Orange_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Magenta_Bed : public Bed
	{
		static constexpr int base_id = 1113;

	public:
		Magenta_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Light_Blue_Bed : public Bed
	{
		static constexpr int base_id = 1129;

	public:
		Light_Blue_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Yellow_Bed : public Bed
	{
		static constexpr int base_id = 1145;

	public:
		Yellow_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Lime_Bed : public Bed
	{
		static constexpr int base_id = 1161;

	public:
		Lime_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Pink_Bed : public Bed
	{
		static constexpr int base_id = 1177;

	public:
		Pink_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Gray_Bed : public Bed
	{
		static constexpr int base_id = 1193;

	public:
		Gray_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Light_Gray_Bed : public Bed
	{
		static constexpr int base_id = 1209;

	public:
		Light_Gray_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Cyan_Bed : public Bed
	{
		static constexpr int base_id = 1225;

	public:
		Cyan_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Purple_Bed : public Bed
	{
		static constexpr int base_id = 1241;

	public:
		Purple_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Blue_Bed : public Bed
	{
		static constexpr int base_id = 1257;

	public:
		Blue_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Brown_Bed : public Bed
	{
		static constexpr int base_id = 1273;

	public:
		Brown_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Green_Bed : public Bed
	{
		static constexpr int base_id = 1289;

	public:
		Green_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Red_Bed : public Bed
	{
		static constexpr int base_id = 1305;

	public:
		Red_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
	class Black_Bed : public Bed
	{
		static constexpr int base_id = 1321;

	public:
		Black_Bed(Facing2D facing, bool occupied, Part part) : Bed(facing, occupied, part) {}

		virtual bool rightClick(BlockEventArgs &args) const override { return sleep(args); }
		UPDATE_BED()

		INHERIT_BLOCKPLACE(placeBed)
			static int getId(Facing2D facing = Facing2D::north, bool occupied = false, Part part = Part::foot) { return base_id + (part | (!occupied << 1) | ((int)facing << 2)); }
	};
}