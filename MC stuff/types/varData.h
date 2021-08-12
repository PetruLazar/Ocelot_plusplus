#pragma once
#include "typedefs.h"

class varInt
{
	int v;
public:
	varInt();
	varInt(int);
	void read(std::fstream&);
	void write(std::fstream&) const;
	void read(char*&);
	void write(char*&) const;
	operator int& ();
	void operator=(int);

	//check if there is a valid varInt in memory with the maximum size of 'max' bytes
	static bool valid(char*, size_t max);
};

class varLong
{
	long long v;
public:
	varLong();
	varLong(long long);
	void read(std::fstream&);
	void write(std::fstream&) const;
	void read(char*&);
	void write(char*&) const;
	operator long long& ();
	void operator=(int64);

	//check if there is a valid varLong in memory with the maximum size of 'max' bytes
	static bool valid(char*, size_t max);
};