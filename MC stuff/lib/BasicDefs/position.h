#pragma once
#include <fstream>
#include "../endian.h"
#include <SFML/System/Vector3.hpp>
#include "apidef.h"

class Position
{
	bigEndian<ull> v;

public:
	BASICDEFS_API Position();
	BASICDEFS_API Position(int x, int y, int z);
	BASICDEFS_API Position(const sf::Vector3i&);

	BASICDEFS_API void read(std::fstream&);
	BASICDEFS_API void write(std::fstream&) const;
	BASICDEFS_API void read(char*&);
	BASICDEFS_API void write(char*&) const;

	BASICDEFS_API int x() const;
	BASICDEFS_API int y() const;
	BASICDEFS_API int z() const;
	BASICDEFS_API sf::Vector3i get() const;

	BASICDEFS_API void setX(int);
	BASICDEFS_API void setY(int);
	BASICDEFS_API void setZ(int);
	BASICDEFS_API void set(sf::Vector3i);
};

