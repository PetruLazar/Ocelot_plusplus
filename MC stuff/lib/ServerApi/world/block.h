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

class SnowyBlock;
class DistanceProperty;
class LeafPersistentProperty;

class BlockState
{
public:
	virtual bool replaceable() const { return false; }

	virtual const SnowyBlock* const snowy() const { return nullptr; }
	virtual const DistanceProperty* const distance() const { return nullptr; }
	virtual const LeafPersistentProperty* const persistent() const { return nullptr; }

	virtual bool rightClick() const { return false; }

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
#include "blocks/solid.h"
#include "blocks/herb.h"
#include "blocks/leaves/leaves.h"
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