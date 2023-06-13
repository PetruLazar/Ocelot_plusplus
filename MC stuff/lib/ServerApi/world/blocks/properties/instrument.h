#pragma once

#include "../../block.h"

namespace Blocks
{
	class InstrumentProperty : virtual public BlockState
	{
	public:
		enum Instrument
		{
			harp,
			basedrum,
			snare,
			hat,
			bass,
			flute,
			bell,
			guitar,
			chime,
			xylophone,
			iron_xylophone,
			cow_bell,
			didgeridoo,
			bit,
			banjo,
			pling
		};

		InstrumentProperty(Instrument instrument) : value(instrument) {}
		virtual const InstrumentProperty* const instrument() const override { return this; }
		Instrument get() const { return value; }

	protected:
		Instrument value;
	};
}