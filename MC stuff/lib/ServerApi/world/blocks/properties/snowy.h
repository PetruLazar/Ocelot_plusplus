#pragma once

#include "../../block.h"

namespace Blocks
{
	class SnowyBlock : virtual public SolidBlock
	{
	public:
		SnowyBlock(bool snowy) : value(snowy) {}
		virtual const SnowyBlock* const snowy() const override { return this; }
		bool get() const { return value; }

		virtual void free() const override { delete this; }

	protected:
		static bool placeSnowy(DECL_BLOCKPLACE_ARGS, int base_id);

		bool value;
	};
}