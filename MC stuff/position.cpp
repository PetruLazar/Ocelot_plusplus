#include "position.h"

Position::Position() { };
Position::Position(int x, int y, int z) : v((ull(x & 0x3ffffff) << 38) | (ull(z & 0x3ffffff) << 12) | ((ull)y & 0xfff)) { }
Position::Position(sf::Vector3i v3) : v((ull(v3.x & 0x3ffffff) << 38) | (ull(v3.z & 0x3ffffff) << 12) | ((ull)v3.y & 0xfff)) { }

void Position::read(std::fstream& fs)
{
	v.read(fs);
}
void Position::write(std::fstream& fs) const
{
	v.write(fs);
}
void Position::read(char*& buffer)
{
	v.read(buffer);
}
void Position::write(char*& buffer) const
{
	v.write(buffer);
}

int Position::x() const
{
	int ret = (ull&)v >> 38;
	if (ret >> 25) return ret ^ 0xfc000000;
	return ret;
}
int Position::y() const
{
	int ret = (ull&)v & 0xfff;
	if (ret >> 11) return ret ^ 0xfffff000;
	return ret;
}
int Position::z() const
{
	int ret = (ull&)v << 26 >> 38;
	if (ret >> 25) return ret ^ 0xfc000000;
	return ret;
}
sf::Vector3i Position::get() const
{
	return sf::Vector3i(x(), y(), z());
}

void Position::setX(int x)
{
	v = v & 0x3fffffffff | ((ull)x << 38);
}
void Position::setY(int y)
{
	v = v & ~0xfff | (y & 0xfff);
}
void Position::setZ(int z)
{
	v = v & 0xffffffc000000fff | (ull(z & 0x3ffffff) << 12);
}
//watch the asm calling
void Position::set(sf::Vector3i v3)
{
	v = (ull(v3.x & 0x3ffffff) << 38) | (ull(v3.z & 0x3ffffff) << 12) | ((ull)v3.y & 0xfff);
}