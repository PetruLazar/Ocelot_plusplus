#pragma once

#include "../../block.h"

namespace Blocks
{
	class OccupiedProperty : virtual public BlockState
	{
	public:

		OccupiedProperty(bool occupied) : value(occupied) {}
		virtual const OccupiedProperty* const occupied() const override { return this; }
		bool get() const { return value; }

	protected:
		bool value;
	};
}