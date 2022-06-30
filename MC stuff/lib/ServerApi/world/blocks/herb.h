#pragma once

#include "../block.h"

class Herb : public BlockState
{
public:
	virtual bool replaceable() const override { return true; }

	virtual void free() const override { delete this; }
};

class Grass : public Herb
{

};
class Fern : public Herb
{

};