#include "bitstream.h"

const char errorInvalidBitsPerEntry[] = "Invalid \"bitsPerEntry\" provided to BitStream constructor.";
const char errorEmptyStream[] = "Cannot extract data from empty stream.";

BitStream::BitStream(byte bitsPerEntry, bool rightPadded) : rightPadded(rightPadded), bitsPerEntry(bitsPerEntry), lastEntry(0), entries(1, 0), sz(1), mask(0xffffffffffffffff << (64 - bitsPerEntry) >> (64 - bitsPerEntry))
{
	if (bitsPerEntry > 64 || !bitsPerEntry)
	{
		BitStream::~BitStream();
		throw errorInvalidBitsPerEntry;
	}
}

void BitStream::clear()
{
	entries.clear();
	entries.push_back(0);
	lastEntry = 0;
}
ull BitStream::size() const
{
	return sz;
}
void BitStream::write(std::fstream& f) const
{
	for (bulong e : entries) e.write(f);
}
void BitStream::write(char*& buffer) const
{
	for (bulong e : entries) e.write(buffer);
}

BitStream& BitStream::operator<<(ull v)
{
	if (lastEntry + bitsPerEntry > 64)
	{
		entries.push_back(v & mask);
		sz++;
		lastEntry = bitsPerEntry;
		return *this;
	}
	ull& entry = entries[sz - 1];
	entry = entry | ((v & mask) << lastEntry);
	lastEntry += bitsPerEntry;
	return *this;
}
BitStream& BitStream::operator>>(ull& v)
{
	if (sz <= 1 && lastEntry < bitsPerEntry) throw errorEmptyStream;
	lastEntry -= bitsPerEntry;
	ull& entry = entries[sz - 1];
	v = entry >> lastEntry;
	entry &= ~(mask << lastEntry);
	if (lastEntry < bitsPerEntry && sz > 1)
	{
		entries.pop_back();
		lastEntry = 64 - (64 % bitsPerEntry);
		sz--;
	}
	return *this;
}