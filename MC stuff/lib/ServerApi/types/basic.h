#pragma once

#include "../types/typedefs.h"
/*#include <Windows.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>*/

//comment or decomment this define to switch between custom new operator and usual new operator
//#define DEBUG_ALLOCATIONS
#ifdef DEBUG_ALLOCATIONS

void* operator new(size_t s);
void* operator new[](size_t s);

void operator delete(void*, size_t);
void operator delete[](void*, size_t);

#endif

//comment or decomment this define to switch to turn on or off chunk debug
//#define CHUNK_DEBUG
#ifdef CHUNK_DEBUG
#define IF_CHUNK_DEBUG(x) x
#else
#define IF_CHUNK_DEBUG(x)
#endif

//comment or decomment this define to switch to turn on or off region debug
//#define REGION_DEBUG
#ifdef REGION_DEBUG
#define IF_REGION_DEBUG(x) x
#else
#define IF_REGION_DEBUG(x)
#endif

const int protocolVersionCurrent = 756; // 1.17.1

/*struct vector2
{
	int x, z;
};
struct vector3
{
	int x, y, z;
};*/

/*
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
*/