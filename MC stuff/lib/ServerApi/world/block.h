#pragma once
#include "../types/apidef.h"
#include "../types/varData.h"
#include "../json.h"
#include "../types/registry.h"
#include "../nbt.h"
#include "../types/item.h"
class World;
#include "../world.h"

enum class BlockFace
{
	bottom,
	top,
	north,
	south,
	west,
	east
};

namespace Blocks
{
	class SnowyBlock;
	class DistanceProperty;
	class LeafPersistentProperty;
	class LevelProperty;
	class AxisProperty;
	class InstrumentProperty;
	class NoteProperty;
	class PoweredProperty;
}

class BlockState
{
public:
	virtual bool replaceable() const { return false; }
	virtual bool isLiquid() const { return false; }

	virtual const Blocks::SnowyBlock* const snowy() const { return nullptr; }
	virtual const Blocks::DistanceProperty* const distance() const { return nullptr; }
	virtual const Blocks::LeafPersistentProperty* const persistent() const { return nullptr; }
	virtual const Blocks::LevelProperty* const level() const { return nullptr; }
	virtual const Blocks::AxisProperty* const axis() const { return nullptr; }
	virtual const Blocks::InstrumentProperty* const instrument() const { return nullptr; }
	virtual const Blocks::NoteProperty* const note() const { return nullptr; }
	virtual const Blocks::PoweredProperty* const powered() const { return nullptr; }

	virtual bool rightClick(World* wld, int x, int y, int z, Item itemId) const { return false; }
	void update(World* wld, int x, int y, int z) const { if (updateRoot(wld, x, y, z)) updateAround(wld, x, y, z); }
	static void updateAround(World* wld, int x, int y, int z);
	virtual bool updateRoot(World* wld, int x, int y, int z) const { return false; }
	virtual bool randomTick(World* wld, int x, int y, int z) const { return false; }

	virtual void free() const = 0;

	static const BlockState* const globalPalette[];
	static constexpr ull globalPaletteSize = 20342;

	/*json* state;

public:
	varInt id;

	SERVER_API BlockState(int id);
	SERVER_API BlockState(const std::string blockName);
	SERVER_API BlockState(const std::string blockName, BlockProperty* properties);
	SERVER_API BlockState(json* blockState);
	SERVER_API BlockState(json& blockState);

	SERVER_API std::string getState(const std::string&) const;
	SERVER_API json* getJsonState() const;
	SERVER_API void setState(const std::string& state, const std::string& value);

	SERVER_API void set(int blockId);
	SERVER_API void set(const std::string& blockName);
	SERVER_API void set(const std::string& blockName, BlockProperty* properties);

	SERVER_API bool operator==(const BlockState&) const;
	SERVER_API void operator=(const BlockState&);*/
};

#pragma warning (push)
#pragma warning (disable : 4250)
#include "blocks/properties/level.h"
#include "blocks/properties/axis.h"
#include "blocks/solid.h"
#include "blocks/herb.h"
#include "blocks/leaves/leaves.h"
#include "blocks/liquids/liquids.h"
#include "blocks/logs/logs.h"
#pragma warning (pop)

//struct blockEntity
//{
//	Byte packedXY;
//	bshort Y;
//	varInt type;
//	nbt_compound* nbt;
//
//	blockEntity(Byte packedXY, bshort Y, varInt type, nbt_compound* nbt) : packedXY(packedXY), Y(Y), type(type), nbt(nbt) {}
//
//	void write(char*& buffer) const
//	{
//		*(buffer++) = packedXY;
//		Y.write(buffer);
//		type.write(buffer);
//		nbt->write(buffer);
//	}
//};

class BlockEntity
{
	Byte packedXZ;
	bshort y;
	varInt type;
	nbt_compound *tags;

public:
	//creation on placement
	//access on right click
	//access on tick
	//access on block break
	SERVER_API BlockEntity(Byte packedXZ, bshort y, varInt type, nbt_compound* tags);
	SERVER_API ~BlockEntity();

	SERVER_API void writeExplicit(char*& buffer);
	SERVER_API void write(char*& buffer);
};