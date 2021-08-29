#pragma once
#include "../typedefs.h"
#include "../endian.h"
#include "apidef.h"

class BitArray;

class BitArrayElement
{
	BitArray* parent;
	ull index;

public:
	BASICDEFS_API BitArrayElement(BitArray* parent, ull index);

	BASICDEFS_API operator ull() const;
	BASICDEFS_API void operator=(ull) const;
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
	BASICDEFS_API BitArray(ull entryCount, byte bitsPerEntry);
	BASICDEFS_API BitArray(ull entryCount, byte bitsPerEntry, ull* values);
	BASICDEFS_API BitArray(ull entryCount, byte bitsPerEntry, uint* values);
	BASICDEFS_API BitArray(ull entryCount, byte bitsPerEntry, byte* values);
	BASICDEFS_API ~BitArray();

	//maybe a getAll and a setAll for speed

	BASICDEFS_API ull getElement(ull);
	BASICDEFS_API void setElement(ull index, ull value);

	BASICDEFS_API ull getCompactedSize() const;
	BASICDEFS_API blong* getCompactedValues() const;
	BASICDEFS_API void changeSize(ull);
	BASICDEFS_API void changeBitsPerEntry(byte);

	BASICDEFS_API BitArrayElement operator[](ull);

	BASICDEFS_API void write(std::fstream&) const;
	BASICDEFS_API void write(char*&) const;
	BASICDEFS_API void read(std::fstream&);
	BASICDEFS_API void read(char*&);
};

class BitArrayReversed
{
	ull entryCount;
	byte bitsPerEntry;
	ull mask;
	ull groupSize;

	ull compactedSize;
	blong* compactedData;
public:
	BASICDEFS_API BitArrayReversed(ull entryCount, byte bitsPerEntry);
	BASICDEFS_API BitArrayReversed(ull entryCount, byte bitsPerEntry, ull* values);
	BASICDEFS_API BitArrayReversed(ull entryCount, byte bitsPerEntry, uint* values);
	BASICDEFS_API BitArrayReversed(ull entryCount, byte bitsPerEntry, byte* values);
	BASICDEFS_API ~BitArrayReversed();

	//maybe a getAll and a setAll for speed

	BASICDEFS_API ull getElement(ull);
	BASICDEFS_API void setElement(ull index, ull value);

	BASICDEFS_API ull getCompactedSize() const;
	BASICDEFS_API blong* getCompactedValues() const;
	BASICDEFS_API void changeSize(ull);
	BASICDEFS_API void changeBitsPerEntry(byte);

	BASICDEFS_API BitArrayElement operator[](ull);

	BASICDEFS_API void write(std::fstream&) const;
	BASICDEFS_API void write(char*&) const;
	BASICDEFS_API void read(std::fstream&);
	BASICDEFS_API void read(char*&);
};