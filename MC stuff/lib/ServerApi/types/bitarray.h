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
	SERVER_API ~BitArray()
	{
		delete[] compactedData;
	}

	//maybe a getAll and a setAll for speed

	SERVER_API ull getElement(ull index)
	{
		return (compactedData[index / groupSize] >> (index % groupSize * bitsPerEntry)) & mask;
	}
	SERVER_API void setElement(ull index, ull value)
	{
		ull compactedArrayElement = index / groupSize,
			long_index = (index % groupSize) * bitsPerEntry;
		compactedData[compactedArrayElement] = compactedData[compactedArrayElement] & ~(mask << long_index) | ((value & mask) << long_index);
	}

	SERVER_API ull getCompactedSize() const { return compactedSize; }
	SERVER_API blong* getCompactedValues() const { return compactedData; }
	SERVER_API void changeSize(ull);
	SERVER_API void changeBitsPerEntry(Byte);

	SERVER_API BitArrayElement operator[](ull);

	SERVER_API void write(std::ostream& f) const
	{
		f.write((char*)compactedData, compactedSize << 3);
		//for (int i = 0; i < compactedSize; i++) compactedData[i].write(f);
	}
	SERVER_API void write(char*& buffer) const
	{
		for (int i = 0; i < compactedSize; i++) compactedData[i].write(buffer);
	}
	SERVER_API void read(std::istream& f)
	{
		f.read((char*)compactedData, compactedSize << 3);
		//for (int i = 0; i < compactedSize; i++) compactedData[i].read(f);
	}
	SERVER_API void read(char*& buffer)
	{
		for (int i = 0; i < compactedSize; i++) compactedData[i].read(buffer);
	}
	SERVER_API void writeAsLight(char*& buffer) const
	{
		for (ull i = 0; i < compactedSize; i++) *(((int64*&)buffer)++) = compactedData[i];
	}
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