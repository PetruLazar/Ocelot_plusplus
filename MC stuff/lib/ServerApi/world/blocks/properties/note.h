#pragma once

#include "../../block.h"

namespace Blocks
{
	// [0, 24]
	class NoteProperty : virtual public BlockState
	{
	public:
		NoteProperty(Byte note) : value(note) {}
		virtual const NoteProperty* const note() const override { return this; }
		Byte get() const { return value; }

	protected:
		Byte value;
	};
}