#pragma once

#include "../../block.h"

namespace Blocks
{
	// water = [0, 15]
	class LevelProperty : virtual public BlockState
	{
	public:
		LevelProperty(Byte level) : value(level) {}
		virtual const Blocks::LevelProperty* const level() const override { return this; }
		Byte get() const { return value; }
		virtual int set(Byte level) const = 0;

	protected:
		Byte value;
	};
}