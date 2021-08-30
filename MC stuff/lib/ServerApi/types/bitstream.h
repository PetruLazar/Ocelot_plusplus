#pragma once

#include "typedefs.h"
#include <vector>
#include "endian.h"
#include "apidef.h"

class BitStream
{
	ull mask;
	bool rightPadded;
	byte lastEntry;
	byte bitsPerEntry;
	std::vector<bulong> entries;
	ull sz;
public:
	SERVER_API BitStream(byte bitsPerEntry, bool rightPadded = false);

	//clear the bit stream
	SERVER_API void clear();
	//are there any more entries in the stream?
	//bool empty() const;
	//get the size of the bitstream (how many longs?)
	SERVER_API ull size() const;
	//write the bitstream entries to file
	SERVER_API void write(std::fstream&) const;
	//write the bitstream entries to buffer
	SERVER_API void write(char*&) const;

	//insert data into the stream
	SERVER_API BitStream& operator<<(ull);
	//extract data from the stream
	SERVER_API BitStream& operator>>(ull&);
};

