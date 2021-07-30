#pragma once
#include "basic.h"

class varInt
{
	int v;
public:
	varInt();
	varInt(int);
	void read(std::fstream&);
	void write(std::fstream&);
	void read(char*&);
	void write(char*&);
	operator int& ();

	static bool valid(char*, size_t);
};

class varLong
{
	long long v;
public:
	varLong();
	varLong(long long);
	void read(std::fstream&);
	void write(std::fstream&);
	void read(char*&);
	void write(char*&);
	operator long long& ();

	static bool valid(char*, size_t);
};