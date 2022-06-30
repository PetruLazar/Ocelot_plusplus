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

	void write(char*&) const;
	//read
	void write(std::ostream&) const;
	void read(std::istream&);
};
class SERVER_API BiomesContainer
{
	void* container;

public:
	BiomesContainer();
	~BiomesContainer();

	int get(int index) const;
	bool set(int index, int value);

	void write(char*&) const;
	//read
	void write(std::ostream&) const;
	void read(std::istream&);
};