#pragma once

#include "../../block.h"

namespace Blocks
{
	class PoweredProperty : virtual public BlockState
	{
	public:
		PoweredProperty(bool powered) : value(powered) {}
		virtual const PoweredProperty* const powered() const override { return this; }
		bool get() const { return value; }

	protected:
		bool value;
	};
}