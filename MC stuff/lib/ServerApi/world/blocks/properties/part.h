#pragma once

#include "../../block.h"

namespace Blocks
{
	class PartProperty : virtual public BlockState
	{
	public:
		enum Part
		{
			head,
			foot
		};
		static inline Part reverse(Part part)
		{
			return Part(-part + 1);
		}

		PartProperty(Part part) : value(part) {}
		virtual const PartProperty* const part() const override { return this; }
		Part get() const { return value; }

	protected:
		Part value;
	};
}