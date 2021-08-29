#pragma once
#include <fstream>
#include "typedefs.h"

#define bshort bigEndian<short>
#define bushort bigEndian<ush>
#define bint bigEndian<int>
#define buint bigEndian<uint>
#define blong bigEndian<int64>
#define bulong bigEndian<ull>
#define bfloat bigEndian<float>
#define bdouble bigEndian<double>

template <class T> class bigEndian
{
	T v;

public:
	bigEndian() : v(0) { }
	bigEndian(T value) : v(value) { }
	void write(std::fstream& os) const
	{
		//write the bytes in reverse
		for (int i = sizeof(T) - 1; i >= 0; i--) os.write((char*)&v + i, 1);
	}
	void read(std::fstream& is)
	{
		//read the bytes in reverse
		for (int i = sizeof(T) - 1; i >= 0; i--) is.read((char*)&v + i, 1);
	}
	void write(char*& buffer) const
	{
		for (int i = sizeof(T) - 1; i >= 0; i--) buffer[i] = ((char*)&v)[sizeof(T) - 1 - i];
		buffer += sizeof(T);
	}
	void read(char*& buffer)
	{
		for (int i = sizeof(T) - 1; i >= 0; i--) ((char*)&v)[i] = buffer[sizeof(T) - 1 - i];
		buffer += sizeof(T);
	}
	//obtain the bytes of the variable as a const char*
	const char* c_str()
	{
		char* p = new char[sizeof(T)];
		for (int i = sizeof(T) - 1; i >= 0; i--) p[sizeof(T) - 1 - i] = *((char*)&v + i);
		return p;
	}
	operator T& ()
	{
		return v;
	}
	operator const T& () const { return v; }
	void operator=(const T& value)
	{
		v = value;
	}
};