#include "varData.h"

varInt::varInt() : v(0) { };
varInt::varInt(int a) : v(a) { };
void varInt::read(std::fstream& is)
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
void varInt::write(std::fstream& os) const
{
	uint c = (uint)v;
	byte b;
	do
	{
		b = c & 0x7f;
		c >>= 7;
		if (c) b |= 0x80;
		os.write((char*)&b, 1);
	} while (c);
}
void varInt::read(char*& buffer)
{
	v = 0;
	int offset = 0;
	byte b = 0;
	do
	{
		b = *(buffer++);
		v |= uint(b & 0x7f) << offset;
		offset += 7;
	} while (b & 0x80);
}
void varInt::write(char*& buffer) const
{
	uint c = (uint)v;
	byte b;
	do
	{
		b = c & 0x7f;
		c >>= 7;
		if (c) b |= 0x80;
		*(buffer++) = b;
	} while (c);
}
varInt::operator int& ()
{
	return v;
}

varLong::varLong() : v(0) { };
varLong::varLong(long long a) : v(a) { };
void varLong::read(std::fstream& is)
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

	/*v = 0;
	int offset = 0;
	byte b;
	do
	{
		is.read((char*)&b, 1);
		v |= b << offset;
		offset += 7;
	} while (b & 0x80);*/
}
void varLong::write(std::fstream& os) const
{
	ull c = (ull)v;
	byte b;
	do
	{
		b = c & 0x7f;
		c >>= 7;
		if (c) b |= 0x80;
		os.write((char*)&b, 1);
	} while (c);
}
void varLong::read(char*& buffer)
{
	v = 0;
	int offset = 0;
	byte b = 0;
	do
	{
		b = *(buffer++);
		v |= ull(b & 0x7f) << offset;
		offset += 7;
	} while (b & 0x80);
}
void varLong::write(char*& buffer) const
{
	ull c = (ull)v;
	byte b;
	do
	{
		b = c & 0x7f;
		c >>= 7;
		if (c) b |= 0x80;
		*(buffer++) = b;
	} while (c);
}
varLong::operator long long& ()
{
	return v;
}

bool varInt::valid(char* buffer, size_t max)
{
	ull size = 1;
	while (*(buffer++) & 0x80) if (++size > max) return false;
	return true;
}
bool varLong::valid(char* buffer, size_t max)
{
	ull size = 1;
	while (*(buffer++) & 0x80) if (++size > max) return false;
	return true;
}