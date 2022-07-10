#pragma once

#include "../../block.h"

namespace Blocks
{
	class AxisProperty : virtual public BlockState
	{
	public:
		enum AxisOrientation
		{
			X,
			Y,
			Z
		};

		AxisProperty(AxisOrientation orientation) : value(orientation) {}
		virtual const AxisProperty* const axis() const override { return this; }
		AxisOrientation get() const { return value; }
		//virtual int set(AxisOrientation) const = 0;

	protected:
		AxisOrientation value;
	};
}