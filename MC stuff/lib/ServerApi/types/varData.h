#pragma once
#include <fstream>
#include "typedefs.h"
#include "apidef.h"

class varInt
{
	int v;
public:
	SERVER_API varInt();
	SERVER_API varInt(int);
	void SERVER_API read(std::fstream&);
	void SERVER_API write(std::fstream&) const;
	void SERVER_API read(char*&);
	void SERVER_API write(char*&) const;
	SERVER_API operator int& ();
	void SERVER_API operator=(int);

	//check if there is a valid varInt in memory with the maximum size of 'max' bytes
	SERVER_API static bool valid(char*, size_t max);
	SERVER_API static ull size(int);
};

class varLong
{
	long long v;
public:
	SERVER_API varLong();
	SERVER_API varLong(long long);
	void SERVER_API read(std::fstream&);
	void SERVER_API write(std::fstream&) const;
	void SERVER_API read(char*&);
	void SERVER_API write(char*&) const;
	SERVER_API operator long long& ();
	void SERVER_API operator=(int64);

	//check if there is a valid varLong in memory with the maximum size of 'max' bytes
	SERVER_API static bool valid(char*, size_t max);
	SERVER_API static ull size(int64);
};