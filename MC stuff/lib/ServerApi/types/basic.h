#pragma once

#include "../types/typedefs.h"
#include "apidef.h"
#include "../../zlib/include/zlib.h"
#include <ctime>

const int protocolVersionCurrent = 756; // 1.18.2
#define ignoreExceptions(x) try {x;} catch (...){}

extern SERVER_API clock_t cycleTime;

//compress/defalte data using zlib
//return true for success, false otherwise
SERVER_API bool zlibCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint& compressedSize, int level = 6, int flush = Z_FINISH);
SERVER_API bool zlibCompressNoAlloc(char* decompressedData, uint decompressedSize, char* compressedData, uint& compressedSize, int level = 6, int flush = Z_FINISH);
//decompress/inflate data using zlib
//return true for success, false otherwise
SERVER_API bool zlibDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush = Z_FINISH);
SERVER_API bool zlibDecompressNoAlloc(char* compressedData, uint compressedSize, char* decompressedData, uint decompressedSize, int flush = Z_FINISH);

//compress/deflate data using gzip
//return true for success, false otherwise
SERVER_API bool gzipCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint& compressedSize, int level = 6, int flush = Z_FINISH);
//decompress/inflate data using gzip
//return true for success, false otherwise
SERVER_API bool gzipDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush = Z_FINISH);