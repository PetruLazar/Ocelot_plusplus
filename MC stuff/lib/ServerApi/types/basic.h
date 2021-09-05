#pragma once

#include "../types/typedefs.h"
#include "apidef.h"
#include "../../zlib/include/zlib.h"
#include <ctime>
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
#define ignoreExceptions(x) try {x;} catch (...){}

extern SERVER_API clock_t cycleTime;

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
SERVER_API bool zlibCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint &compressedSize, int level = 6, int flush = Z_FINISH);
SERVER_API bool zlibCompressNoAlloc(char* decompressedData, uint decompressedSize, char* compressedData, uint& compressedSize, int level = 6, int flush = Z_FINISH);
//decompress/inflate data using zlib
//return true for success, false otherwise
SERVER_API bool zlibDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush = Z_FINISH);
SERVER_API bool zlibDecompressNoAlloc(char* compressedData, uint compressedSize, char* decompressedData, uint decompressedSize, int flush = Z_FINISH);

//compress/deflate data using gzip
//return true for success, false otherwise
SERVER_API bool gzipCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint &compressedSize, int level = 6, int flush = Z_FINISH);
//decompress/inflate data using gzip
//return true for success, false otherwise
SERVER_API bool gzipDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush = Z_FINISH);