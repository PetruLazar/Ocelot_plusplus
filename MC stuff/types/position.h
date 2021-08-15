#pragma once
#include "endian.h"

class Position
{
	bigEndian<ull> v;

public:
	Position();
	Position(int x, int y, int z);
	Position(const sf::Vector3i&);

	void read(std::fstream&);
	void write(std::fstream&) const;
	void read(char*&);
	void write(char*&) const;

	int x() const;
	int y() const;
	int z() const;
	sf::Vector3i get() const;

	void setX(int);
	void setY(int);
	void setZ(int);
	void set(sf::Vector3i);
};

