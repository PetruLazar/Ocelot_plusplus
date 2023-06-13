#pragma once

#include "../../block.h"

namespace Blocks
{
	class TriggeredProperty : virtual public BlockState
	{
	public:

		TriggeredProperty(bool triggered) : value(triggered) {}
		virtual const TriggeredProperty* const triggered() const override { return this; }
		bool get() const { return value; }

	protected:
		bool value;
	};
}