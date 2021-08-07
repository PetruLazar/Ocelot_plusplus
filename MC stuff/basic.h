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

//comment or decomment this define to switch between custom new operator and usual new operator
//#define DEBUG_ALLOCATIONS

#ifdef DEBUG_ALLOCATIONS

void* operator new(size_t s);
void* operator new[](size_t s);

void operator delete(void*, size_t);
void operator delete[](void*, size_t);

#endif

typedef unsigned char byte;

typedef unsigned short ush;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long int64;

enum class gamemode : byte
{
	survival,
	creative,
	adventure,
	spectator,

	//the 'previous gamemode' default value (-1)
	none = 0xff
};

typedef unsigned short Port;
const int protocolVersionCurrent = 756; // 1.17.1

/*struct vector2
{
	int x, z;
};
struct vector3
{
	int x, y, z;
};*/

//compress/defalte data using zlib
//return true for success, false otherwise
bool zlibCompress(char* src, uint size, char* dest, uint maxSize, uint & trueSize, int level = 6, int flush = Z_FINISH);
//decompress/inflate data using zlib
//return true for success, false otherwise
bool zlibDecompress(char* src, uint size, char* dest, uint maxSize, uint& trueSize, int flush = Z_FINISH);

//compress/deflate data using gzip
//return true for success, false otherwise
bool gzipCompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int level = 6, int flush = Z_FINISH);
//decompress/inflate data using gzip
//return true for success, false otherwise
bool gzipDecompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int flush = Z_FINISH);