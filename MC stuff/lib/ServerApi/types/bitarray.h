#pragma once

#include "typedefs.h"
#include "endian.h"
#include "apidef.h"

class BitArray;

class BitArrayElement
{
	BitArray* parent;
	ull index;

public:
	SERVER_API BitArrayElement(BitArray* parent, ull index);

	SERVER_API operator ull() const;
	SERVER_API void operator=(ull) const;
};

class BitArray
{
	ull entryCount;
	Byte bitsPerEntry;
	ull mask;
	ull groupSize;

	ull compactedSize;
	blong* compactedData;
public:
	SERVER_API BitArray(ull entryCount, Byte bitsPerEntry);
	SERVER_API BitArray(ull entryCount, Byte bitsPerEntry, ull* values);
	SERVER_API BitArray(ull entryCount, Byte bitsPerEntry, uint* values);
	SERVER_API BitArray(ull entryCount, Byte bitsPerEntry, Byte* values);
	SERVER_API ~BitArray();

	//maybe a getAll and a setAll for speed

	SERVER_API ull getElement(ull);
	SERVER_API void setElement(ull index, ull value);

	SERVER_API ull getCompactedSize() const;
	SERVER_API blong* getCompactedValues() const;
	SERVER_API void changeSize(ull);
	SERVER_API void changeBitsPerEntry(Byte);

	SERVER_API BitArrayElement operator[](ull);

	SERVER_API void write(std::ostream&) const;
	SERVER_API void write(char*&) const;
	SERVER_API void read(std::istream&);
	SERVER_API void read(char*&);
};

class BitArrayReversed
{
	ull entryCount;
	Byte bitsPerEntry;
	ull mask;
	ull groupSize;

	ull compactedSize;
	blong* compactedData;
public:
	SERVER_API BitArrayReversed(ull entryCount, Byte bitsPerEntry);
	SERVER_API BitArrayReversed(ull entryCount, Byte bitsPerEntry, ull* values);
	SERVER_API BitArrayReversed(ull entryCount, Byte bitsPerEntry, uint* values);
	SERVER_API BitArrayReversed(ull entryCount, Byte bitsPerEntry, Byte* values);
	SERVER_API ~BitArrayReversed();

	//maybe a getAll and a setAll for speed

	SERVER_API ull getElement(ull);
	SERVER_API void setElement(ull index, ull value);

	SERVER_API ull getCompactedSize() const;
	SERVER_API blong* getCompactedValues() const;
	SERVER_API void changeSize(ull);
	SERVER_API void changeBitsPerEntry(Byte);

	SERVER_API BitArrayElement operator[](ull);

	SERVER_API void write(std::ostream&) const;
	SERVER_API void write(char*&) const;
	SERVER_API void read(std::istream&);
	SERVER_API void read(char*&);
};