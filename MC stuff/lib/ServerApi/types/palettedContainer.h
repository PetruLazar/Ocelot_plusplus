#pragma once

#include <vector>
#include "bitarray.h"
#include "varData.h"

//template <int, int, int>
//class PalettedContainer;

class SERVER_API BlockStatesContainer
{
	void* container;

public:
	BlockStatesContainer();
	~BlockStatesContainer();

	int get(int index) const;
	bool set(int index, int value);

	//read
	//write
};
class SERVER_API BiomesContainer
{
	void* container;

public:
	BiomesContainer();
	~BiomesContainer();

	int get(int index) const;
	bool set(int index, int value);

	//read
	//write
};