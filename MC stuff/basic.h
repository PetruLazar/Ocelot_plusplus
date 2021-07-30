#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <zlib.h>
#include <vector>
#include <SFML/Network.hpp>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define null *nullptr;
#define sockStat sf::Socket::Status

typedef unsigned char byte;

typedef unsigned short ush;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long int64;

typedef unsigned short Port;
const int protocolVersionCurrent = 756; // 1.17.1
const Port port = 25565;

/*struct vector2
{
	int x, z;
};
struct vector3
{
	int x, y, z;
};*/

bool zlibCompress(char* src, uint size, char* dest, uint maxSize, uint& trueSize, int level = 6, int flush = Z_FINISH);
bool zlibDecompress(char* src, uint size, char* dest, uint maxSize, uint& trueSize, int flush = Z_FINISH);

bool gzipCompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int level = 6, int flush = Z_FINISH);
bool gzipDecompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int flush = Z_FINISH);