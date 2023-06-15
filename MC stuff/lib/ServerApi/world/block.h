#pragma once
#include "../types/apidef.h"
#include "../types/varData.h"
#include "../json.h"
#include "../types/registry.h"
#include "../nbt.h"
#include "../types/item.h"
class World;
#include "../world.h"
class Player;
//#include "../player/player.h"

//#define DECL_RIGHTCLICK_ARGS
//#define PASS_RIGHTCLICK_ARGS
//#define DECL_BLOCKPLACE_ARGS World* wld, int x, int y, int z, float curX, float curY, float curZ, float playerYaw, float playerPitch, BlockFace face, int currentBlockId
//#define PASS_BLOCKPLACE_ARGS wld, x, y, z, curX, curY, curZ, playerYaw, playerPitch, face, currentBlockId
#define INHERIT_BLOCKPLACE(baseFunc) static bool place(BlockEventArgs &args) { return baseFunc(args, base_id); }

enum class BlockFace
{
	bottom,
	top,
	north,
	south,
	west,
	east
};

class BlockState;

struct BlockEventArgs
{
	World* world;
	Player* player;
	int x, y, z;
	float curX, curY, curZ,
		playerYaw, playerPitch;
	BlockFace face;
	Item heldItem;
	int currentBlockId;
	const BlockState* currentBlock;

	inline BlockEventArgs(World* wld, Player* p,
		int x, int y, int z,
		float curX, float curY, float curZ,
		float playerYaw, float playerPitch,
		BlockFace face, Item heldItem,
		int currentBlockId, const BlockState* currentBlock)
		: world(wld), player(p), x(x), y(y), z(z),
		curX(curX), curY(curY), curZ(curZ),
		playerYaw(playerYaw), playerPitch(playerPitch),
		face(face), heldItem(heldItem),
		currentBlockId(currentBlockId), currentBlock(currentBlock)
	{}
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
	class FacingProperty;
	class TriggeredProperty;
	class PartProperty;
	class OccupiedProperty;

	enum class Transparency
	{
		solid,
		transparent,
		partial
	};
}

class BlockState
{
public:
	virtual bool replaceable() const { return false; }
	virtual bool isLiquid() const { return false; }

	virtual Byte lightEmitted() const { return 0; };
	virtual Blocks::Transparency getTransparency(BlockFace) const { return Blocks::Transparency::transparent; };

	virtual const Blocks::SnowyBlock* const snowy() const { return nullptr; }
	virtual const Blocks::DistanceProperty* const distance() const { return nullptr; }
	virtual const Blocks::LeafPersistentProperty* const persistent() const { return nullptr; }
	virtual const Blocks::LevelProperty* const level() const { return nullptr; }
	virtual const Blocks::AxisProperty* const axis() const { return nullptr; }
	virtual const Blocks::InstrumentProperty* const instrument() const { return nullptr; }
	virtual const Blocks::NoteProperty* const note() const { return nullptr; }
	virtual const Blocks::PoweredProperty* const powered() const { return nullptr; }
	virtual const Blocks::FacingProperty* const facing() const { return nullptr; }
	virtual const Blocks::TriggeredProperty* const triggered() const { return nullptr; }
	virtual const Blocks::PartProperty* const part() const { return nullptr; }
	virtual const Blocks::OccupiedProperty* const occupied() const { return nullptr; }

	virtual bool rightClick(BlockEventArgs &args) const { return false; }
	void update(World* wld, int x, int y, int z) const { if (updateRoot(wld, x, y, z)) updateAround(wld, x, y, z); }
	static void updateAround(World* wld, int x, int y, int z);
	virtual bool updateRoot(World* wld, int x, int y, int z) const { return false; }
	virtual bool randomTick(World* wld, int x, int y, int z) const { return false; }

	virtual void free() const = 0;

	static constexpr ull globalPaletteSize = 20342;
	static const BlockState* const globalPalette[globalPaletteSize];

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
#include "blocks/properties/facing.h"
#include "blocks/properties/part.h"
#include "blocks/properties/occupied.h"
#include "blocks/properties/triggered.h"
#include "blocks/solid.h"
#include "blocks/herb.h"
#include "blocks/leaves/leaves.h"
#include "blocks/liquids/liquids.h"
#include "blocks/logs/logs.h"
#include "blocks/bed.h"
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
public:
	Byte packedXZ;
	bshort y;
	varInt type;
	nbt_compound *tags;

	//creation on placement
	//access on right click
	//access on tick
	//access on block break
	inline BlockEntity() {}
	SERVER_API BlockEntity(Byte packedXZ, bshort y, varInt type, nbt_compound* tags);
	SERVER_API ~BlockEntity();

	SERVER_API void writeExplicit(char*& buffer);
	SERVER_API void write(char*& buffer);

	SERVER_API void read(std::istream &is);
	SERVER_API void write(std::ostream &os);
};