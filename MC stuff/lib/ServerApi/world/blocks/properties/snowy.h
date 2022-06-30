#pragma once

#include "../../block.h"

class SnowyBlock : virtual public BlockState
{
public: 
	SnowyBlock(bool snowy) : value(snowy) {}
	virtual const SnowyBlock* const snowy() const override { return this; }
	bool get() const { return value; }

	virtual void free() const override { delete this; }

protected:
	static bool placeSnowy(World* wld, int x, int y, int z, float curX, float curY, float curZ, BlockFace face, const BlockState& currentBlock, int base_id);

	bool value;
};