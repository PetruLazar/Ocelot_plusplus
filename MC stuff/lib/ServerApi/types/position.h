#pragma once
#include <fstream>
#include "endian.h"
#include <SFML/System/Vector3.hpp>
#include "apidef.h"

class Position
{
	bigEndian<ull> v;

public:
	SERVER_API Position();
	SERVER_API Position(int x, int y, int z);
	SERVER_API Position(const sf::Vector3i&);

	SERVER_API void read(std::fstream&);
	SERVER_API void write(std::fstream&) const;
	SERVER_API void read(char*&);
	SERVER_API void write(char*&) const;

	SERVER_API int x() const;
	SERVER_API int y() const;
	SERVER_API int z() const;
	SERVER_API sf::Vector3i get() const;

	SERVER_API void setX(int);
	SERVER_API void setY(int);
	SERVER_API void setZ(int);
	SERVER_API void set(sf::Vector3i);

	SERVER_API void incX();
	SERVER_API void incY();
	SERVER_API void incZ();
	SERVER_API void decX();
	SERVER_API void decY();
	SERVER_API void decZ();
};

