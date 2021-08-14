#pragma once
#include "typedefs.h"
#include "endian.h"

class BitStream
{
	ull mask;
	bool rightPadded;
	byte lastEntry;
	byte bitsPerEntry;
	std::vector<bulong> entries;
	ull sz;
public:
	BitStream(byte bitsPerEntry, bool rightPadded);

	//clear the bit stream
	void clear();
	//get the size of the bitstream (how many longs?)
	ull size() const;
	//write the bitstream entries to file
	void write(std::fstream&) const;
	//write the bitstream entries to buffer
	void write(char*&) const;

	//insert data into the stream
	BitStream& operator<<(ull);
	//extract data from the stream
	BitStream& operator>>(ull&);
};

