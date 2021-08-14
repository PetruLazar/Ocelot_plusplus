#pragma once
#include "typedefs.h"
#include "endian.h"

class BitArray;

class BitArrayElement
{
	BitArray* parent;
	ull index;

public:
	BitArrayElement(BitArray* parent, ull index);

	operator ull() const;
	void operator=(ull) const;
};

class BitArray
{
	ull entryCount;
	byte bitsPerEntry;
	ull mask;
	ull groupSize;

	ull compactedSize;
	blong* compactedData;
public:
	BitArray(ull entryCount, byte bitsPerEntry, ull* values = nullptr);

	//maybe a getAll and a setAll for speed

	ull getElement(ull);
	void setElement(ull index, ull value);

	ull getCompactedSize() const;
	void changeSize(ull);
	void changeBitsPerEntry(byte);

	BitArrayElement operator[](ull);

	void write(std::fstream&) const;
	void write(char*&) const;
	void read(std::fstream&);
	void read(char*&);
};

