#pragma once

#include "../../block.h"

// [1, 7]
class DistanceProperty : virtual public BlockState
{
public:
	DistanceProperty(Byte distance) : value(distance) {}
	virtual const DistanceProperty* const distance() const override { return this; }
	Byte get() const { return value; }

protected:
	Byte value;
};