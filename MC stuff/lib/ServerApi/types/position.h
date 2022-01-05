#pragma once
#include <fstream>
#include "endian.h"
#include <SFML/System/Vector3.hpp>
#include "apidef.h"

class Position
{
	bigEndian<ull> v;

public:
	SERVER_API Position() {}
	SERVER_API Position(int x, int y, int z) : v((ull(x & 0x3ffffff) << 38) | (ull(z & 0x3ffffff) << 12) | ((ull)y & 0xfff)) {}
	SERVER_API Position(const sf::Vector3i& v3) : v((ull(v3.x & 0x3ffffff) << 38) | (ull(v3.z & 0x3ffffff) << 12) | ((ull)v3.y & 0xfff)) {}

	SERVER_API void read(std::fstream& fs)
	{
		v.read(fs);
	}
	SERVER_API void write(std::fstream& fs) const
	{
		v.write(fs);
	}
	SERVER_API void read(char*& buffer)
	{
		v.read(buffer);
	}
	SERVER_API void write(char*& buffer) const
	{
		v.write(buffer);
	}

	SERVER_API int x() const
	{
		int ret = (ull&)v >> 38;
		if (ret >> 25) return ret ^ 0xfc000000;
		return ret;
	}
	SERVER_API int y() const
	{
		int ret = (ull&)v & 0xfff;
		if (ret >> 11) return ret ^ 0xfffff000;
		return ret;
	}
	SERVER_API int z() const
	{
		int ret = (ull&)v << 26 >> 38;
		if (ret >> 25) return ret ^ 0xfc000000;
		return ret;
	}
	SERVER_API sf::Vector3i get() const
	{
		return sf::Vector3i(x(), y(), z());
	}

	SERVER_API void setX(int x)
	{
		v = v & 0x3fffffffff | ((ull)x << 38);
	}
	SERVER_API void setY(int y)
	{
		v = v & ~0xfff | (y & 0xfff);
	}
	SERVER_API void setZ(int z)
	{
		v = v & 0xffffffc000000fff | (ull(z & 0x3ffffff) << 12);
	}
	SERVER_API void set(const sf::Vector3i& v3)
	{
		v = (ull(v3.x & 0x3ffffff) << 38) | (ull(v3.z & 0x3ffffff) << 12) | ((ull)v3.y & 0xfff);
	}

	SERVER_API void incX()
	{
		setX(x() + 1);
	}
	SERVER_API void incY()
	{
		setY(y() + 1);
	}
	SERVER_API void incZ()
	{
		setZ(z() + 1);
	}
	SERVER_API void decX()
	{
		setX(x() - 1);
	}
	SERVER_API void decY()
	{
		setY(y() - 1);
	}
	SERVER_API void decZ()
	{
		setZ(z() - 1);
	}
};