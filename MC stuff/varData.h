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
	void write(std::fstream&);
	void read(char*&);
	void write(char*&);
	operator long long& ();

	//check if there is a valid varLong in memory with the maximum size of 'max' bytes
	static bool valid(char*, size_t max);
};