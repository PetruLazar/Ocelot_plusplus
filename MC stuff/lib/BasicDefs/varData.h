#pragma once
#include <fstream>
#include "../typedefs.h"
#include "apidef.h"

class varInt
{
	int v;
public:
	BASICDEFS_API varInt();
	BASICDEFS_API varInt(int);
	BASICDEFS_API void read(std::fstream&);
	BASICDEFS_API void write(std::fstream&) const;
	BASICDEFS_API void read(char*&);
	BASICDEFS_API void write(char*&) const;
	BASICDEFS_API operator int& ();
	BASICDEFS_API void operator=(int);

	//check if there is a valid varInt in memory with the maximum size of 'max' bytes
	BASICDEFS_API static bool valid(char*, size_t max);
};

class varLong
{
	long long v;
public:
	BASICDEFS_API varLong();
	BASICDEFS_API varLong(long long);
	BASICDEFS_API void read(std::fstream&);
	BASICDEFS_API void write(std::fstream&) const;
	BASICDEFS_API void read(char*&);
	BASICDEFS_API void write(char*&) const;
	BASICDEFS_API operator long long& ();
	BASICDEFS_API void operator=(int64);

	//check if there is a valid varLong in memory with the maximum size of 'max' bytes
	BASICDEFS_API static bool valid(char*, size_t max);
};