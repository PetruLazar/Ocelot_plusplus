#pragma once
#include "basic.h"

#define bint bigEndian<int>
#define buint bigEndian<uint>
#define blong bigEndian<int64>
#define bulong bigEndian<ull>

template <class T> class bigEndian
{
	T v;

public:
	bigEndian() : v(0) { }
	bigEndian(T value) : v(value) { }
	//write to file
	void write(std::fstream& os)
	{
		//write the bytes in reverse
		for (int i = sizeof(T) - 1; i >= 0; i--) os.write((char*)&v + i, 1);
	}
	//read from file
	void read(std::fstream& is)
	{
		//read the bytes in reverse
		for (int i = sizeof(T) - 1; i >= 0; i--) is.read((char*)&v + i, 1);
	}
	//write to buffer
	void write(char*& buffer)
	{
		for (int i = sizeof(T) - 1; i >= 0; i--) buffer[i] = ((char*)&v)[sizeof(T) - 1 - i];
		buffer += sizeof(T);
	}
	//read from buffer
	void read(char*& buffer)
	{
		for (int i = sizeof(T) - 1; i >= 0; i--) ((char*)&v)[i] = buffer[sizeof(T) - 1 - i];
		buffer += sizeof(T);
	}
	const char* c_str()
	{
		char* p = new char[sizeof(T)];
		for (int i = sizeof(T) - 1; i >= 0; i--) p[sizeof(T) - 1 - i] = *((char*)&v + i);
		return p;
	}
	/*void chEnd()
	{
		char* loc = (char*)&v, * copy = ((char*)new int(v)) - 1;
		for (int i = 0; i < sizeof(T); i++) loc[i] = copy[sizeof(T) - i];
		delete (int*)(copy + 1);
	}*/
	operator T& ()
	{
		return v;
	}
};