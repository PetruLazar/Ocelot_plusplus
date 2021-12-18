#include "bitarray.h"

BitArrayElement::BitArrayElement(BitArray* parent, ull index) : parent(parent), index(index) {}

BitArrayElement::operator ull() const { return parent->getElement(index); }
void BitArrayElement::operator=(ull v) const { parent->setElement(index, v); }

BitArray::BitArray(ull entryCount, Byte bitsPerEntry) : entryCount(entryCount), bitsPerEntry(bitsPerEntry), mask(0xffffffffffffffff << (64 - bitsPerEntry) >> (64 - bitsPerEntry)), groupSize(64 / bitsPerEntry), compactedSize((entryCount - 1) / groupSize + 1), compactedData(new blong[compactedSize]) {}
BitArray::BitArray(ull entryCount, Byte bitsPerEntry, ull* values) : entryCount(entryCount), bitsPerEntry(bitsPerEntry), mask(0xffffffffffffffff << (64 - bitsPerEntry) >> (64 - bitsPerEntry)), groupSize(64 / bitsPerEntry), compactedSize((entryCount - 1) / groupSize + 1), compactedData(new blong[compactedSize])
{
	for (int i = 0; i < entryCount; i++) setElement(i, values[i]);
}
BitArray::BitArray(ull entryCount, Byte bitsPerEntry, uint* values) : entryCount(entryCount), bitsPerEntry(bitsPerEntry), mask(0xffffffffffffffff << (64 - bitsPerEntry) >> (64 - bitsPerEntry)), groupSize(64 / bitsPerEntry), compactedSize((entryCount - 1) / groupSize + 1), compactedData(new blong[compactedSize])
{
	for (int i = 0; i < entryCount; i++) setElement(i, values[i]);
}
BitArray::BitArray(ull entryCount, Byte bitsPerEntry, Byte* values) : entryCount(entryCount), bitsPerEntry(bitsPerEntry), mask(0xffffffffffffffff << (64 - bitsPerEntry) >> (64 - bitsPerEntry)), groupSize(64 / bitsPerEntry), compactedSize((entryCount - 1) / groupSize + 1), compactedData(new blong[compactedSize])
{
	for (int i = 0; i < entryCount; i++) setElement(i, values[i]);
}
BitArray::~BitArray()
{
	delete[] compactedData;
}

ull BitArray::getElement(ull i)
{
	return (compactedData[i / groupSize] >> (i % groupSize * bitsPerEntry)) & mask;
}
void BitArray::setElement(ull i, ull value)
{
	ull compactedArrayElement = i / groupSize,
		long_index = (i % groupSize) * bitsPerEntry;
	compactedData[compactedArrayElement] = compactedData[compactedArrayElement] & ~(mask << long_index) | ((value & mask) << long_index);
}

ull BitArray::getCompactedSize() const { return compactedSize; }
blong* BitArray::getCompactedValues() const { return compactedData; }
void BitArray::changeSize(ull newEntryCount)
{
	entryCount = newEntryCount;
	ull newCompactedSize = (entryCount - 1) / groupSize + 1;
	blong* newCompactedData = new blong[newCompactedSize];
	for (int64 i = (compactedSize < newCompactedSize ? compactedSize : newCompactedSize) - 1; i >= 0; i--) newCompactedData[i] = compactedData[i];
	delete[] compactedData;
	compactedSize = newCompactedSize;
	compactedData = newCompactedData;
}
void BitArray::changeBitsPerEntry(Byte newBitsPerEntry)
{
	ull* values = new ull[entryCount];
	for (int i = 0; i < entryCount; i++) values[i] = getElement(i);

	bitsPerEntry = newBitsPerEntry;
	mask = 0xffffffffffffffff << (64 - bitsPerEntry) >> (64 - bitsPerEntry);
	groupSize = 64 / bitsPerEntry;
	compactedSize = (entryCount - 1) / groupSize + 1;
	delete[] compactedData;
	compactedData = new blong[compactedSize];
	for (int i = 0; i < entryCount; i++) setElement(i, values[i]);
	delete[] values;
}

BitArrayElement BitArray::operator[](ull i)
{
	return BitArrayElement(this, i);
}

void BitArray::write(std::ostream& f) const
{
	f.write((char*)compactedData, compactedSize << 3);
	//for (int i = 0; i < compactedSize; i++) compactedData[i].write(f);
}
void BitArray::write(char*& buffer) const
{
	for (int i = 0; i < compactedSize; i++) compactedData[i].write(buffer);
}
void BitArray::read(std::istream& f)
{
	f.read((char*)compactedData, compactedSize << 3);
	//for (int i = 0; i < compactedSize; i++) compactedData[i].read(f);
}
void BitArray::read(char*& buffer)
{
	for (int i = 0; i < compactedSize; i++) compactedData[i].read(buffer);
}