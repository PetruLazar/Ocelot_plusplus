#pragma once

#include "properties/snowy.h"

class SolidBlock : public BlockState
{
protected:
	static bool placeSolid(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock, int base_id);

public:
	virtual void free() const override { delete this; }
};

class Air : public SolidBlock
{
	static constexpr int base_id = 0;
public:
	virtual bool replaceable() const override { return true; }

	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return false; }
	static int getId() { return base_id; }
};
class Stone : public SolidBlock
{
	static constexpr int base_id = 1;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Granite : public SolidBlock
{
	static constexpr int base_id = 2;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Polished_Granite : public SolidBlock
{
	static constexpr int base_id = 3;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Diorite : public SolidBlock
{
	static constexpr int base_id = 4;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Polished_Diorite : public SolidBlock
{
	static constexpr int base_id = 5;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Andesite : public SolidBlock
{
	static constexpr int base_id = 6;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Polished_Andesite : public SolidBlock
{
	static constexpr int base_id = 7;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Grass_Block : public SnowyBlock
{
	static constexpr int base_id = 8;

public:
	Grass_Block(bool snowy) : SnowyBlock(snowy) {}

	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSnowy(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId(bool snowy) { return base_id + !snowy; }
};
class Dirt : public SolidBlock
{
	static constexpr int base_id = 10;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Coarse_Dirt : public SolidBlock
{
	static constexpr int base_id = 11;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};
class Podzol : public SnowyBlock
{
	static constexpr int base_id = 12;

public:
	Podzol(bool snowy) : SnowyBlock(snowy) {}

	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSnowy(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId(bool snowy) { return base_id + !snowy; }
};
class Cobblestone : public SolidBlock
{
	static constexpr int base_id = 14;

public:
	static bool place(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock) { return placeSolid(wld, x, y, z, curX, curY, curZ, face, currentBlock, base_id); }
	static int getId() { return base_id; }
};