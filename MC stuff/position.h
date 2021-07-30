#pragma once
#include "endian.h"

class Position
{
	bigEndian<ull> v;

public:
	Position();
	Position(int, int, int);
	Position(sf::Vector3i);

	void read(std::fstream&);
	void write(std::fstream&);
	void read(char*&);
	void write(char*&);

	int x();
	int y();
	int z();
	sf::Vector3i get();

	void setX(int);
	void setY(int);
	void setZ(int);
	void set(sf::Vector3i);
};

