#pragma once
#include <fstream>
#include "typedefs.h"
#include "apidef.h"

class varInt
{
	int v;
public:
	SERVER_API varInt() : v(0) {}
	SERVER_API varInt(int value) : v(value) {}
	void SERVER_API read(std::fstream& is)
	{
		v = 0;
		int offset = 0;
		uint b = 0;
		do
		{
			is.read((char*)&b, 1);
			v |= (b & 0x7f) << offset;
			offset += 7;
		} while (b & 0x80);
	}
	void SERVER_API write(std::fstream& os) const
	{
		uint c = (uint)v;
		Byte b;
		do
		{
			b = c & 0x7f;
			c >>= 7;
			if (c) b |= 0x80;
			os.write((char*)&b, 1);
		} while (c);
	}
	void SERVER_API read(char*& buffer)
	{
		v = 0;
		int offset = 0;
		Byte b = 0;
		do
		{
			b = *(buffer++);
			v |= uint(b & 0x7f) << offset;
			offset += 7;
		} while (b & 0x80);
	}
	void SERVER_API write(char*& buffer) const
	{
		uint c = (uint)v;
		Byte b;
		do
		{
			b = c & 0x7f;
			c >>= 7;
			if (c) b |= 0x80;
			*(buffer++) = b;
		} while (c);
	}
	SERVER_API operator int& () { return v; }
	SERVER_API operator int() const { return v; }
	void SERVER_API operator=(int other) { v = other; }

	//check if there is a valid varInt in memory with the maximum size of 'max' Bytes
	SERVER_API static bool valid(char* buffer, size_t max)
	{
		ull size = 1;
		while (*(buffer++) & 0x80) if (++size > max) return false;
		return true;
	}
	SERVER_API static ull size(int value)
	{
		ull ret = 0;
		uint v = (uint)value;
		do ret++; while (v >>= 7);
		return ret;
	}
};

class varLong
{
	long long v;
public:
	SERVER_API varLong() : v(0) {}
	SERVER_API varLong(long long value) : v(value) {}
	void SERVER_API read(std::fstream& is)
	{
		v = 0;
		int offset = 0;
		ull b = 0;
		do
		{
			is.read((char*)&b, 1);
			v |= (b & 0x7f) << offset;
			offset += 7;
		} while (b & 0x80);
	}
	void SERVER_API write(std::fstream& os) const
	{
		ull c = (ull)v;
		Byte b;
		do
		{
			b = c & 0x7f;
			c >>= 7;
			if (c) b |= 0x80;
			os.write((char*)&b, 1);
		} while (c);
	}
	void SERVER_API read(char*& buffer)
	{
		v = 0;
		int offset = 0;
		Byte b = 0;
		do
		{
			b = *(buffer++);
			v |= ull(b & 0x7f) << offset;
			offset += 7;
		} while (b & 0x80);
	}
	void SERVER_API write(char*& buffer) const
	{
		ull c = (ull)v;
		Byte b;
		do
		{
			b = c & 0x7f;
			c >>= 7;
			if (c) b |= 0x80;
			*(buffer++) = b;
		} while (c);
	}
	SERVER_API operator long long& () { return v; }
	SERVER_API operator long long() const { return v; }
	void SERVER_API operator=(int64 other) { v = other; }

	//check if there is a valid varLong in memory with the maximum size of 'max' Bytes
	SERVER_API static bool valid(char* buffer, size_t max)
	{
		ull size = 1;
		while (*(buffer++) & 0x80) if (++size > max) return false;
		return true;
	}
	SERVER_API static ull size(int64 value)
	{
		ull ret = 0;
		ull v = (ull)value;
		do ret++; while (v >>= 7);
		return ret;
	}
};