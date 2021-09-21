#include "basic.h"

clock_t cycleTime;

const int Z_DEFAULT_MEMLEVEL = 8, Z_GZIP_HEADER = 16;

void initStream(z_streamp str, char* src, uint srcSize, char* dest, uint destSize)
{
	str->opaque = Z_NULL;
	str->zalloc = Z_NULL;
	str->zfree = Z_NULL;

	str->next_in = (Byte*)src;
	str->avail_in = srcSize;
	str->next_out = (Byte*)dest;
	str->avail_out = destSize;
}

bool zlibCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint& compressedSize, int level, int flush)
{
	compressedData = new char[decompressedSize];
	z_streamp str = new z_stream;
	initStream(str, decompressedData, decompressedSize, compressedData, compressedSize);

	deflateInit(str, level);
	int status = deflate(str, flush);
	deflateEnd(str);

	compressedSize = str->total_out;
	delete str;
	return status == Z_STREAM_END;
}
bool zlibCompressNoAlloc(char* decompressedData, uint decompressedSize, char* compressedData, uint& compressedSize, int level, int flush)
{
	z_streamp str = new z_stream;
	initStream(str, decompressedData, decompressedSize, compressedData, compressedSize);

	deflateInit(str, level);
	int status = deflate(str, flush);
	deflateEnd(str);

	compressedSize = str->total_out;
	delete str;
	return status == Z_STREAM_END;
}
bool zlibDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush)
{
	decompressedData = new char[decompressedSize];
	z_streamp str = new z_stream;
	initStream(str, compressedData, compressedSize, decompressedData, decompressedSize);

	inflateInit(str);
	int status = inflate(str, flush);
	inflateEnd(str);

	delete str;
	return status == Z_STREAM_END;
}
bool zlibDecompressNoAlloc(char* compressedData, uint compressedSize, char* decompressedData, uint decompressedSize, int flush)
{
	z_streamp str = new z_stream;
	initStream(str, compressedData, compressedSize, decompressedData, decompressedSize);

	inflateInit(str);
	int status = inflate(str, flush);
	inflateEnd(str);

	delete str;
	return status == Z_STREAM_END;
}

bool gzipCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint& compressedSize, int level, int flush)
{
	compressedData = new char[decompressedSize];
	z_streamp str = new z_stream;
	initStream(str, decompressedData, decompressedSize, compressedData, compressedSize);

	deflateInit2(str, level, Z_DEFLATED, 15 | Z_GZIP_HEADER, Z_DEFAULT_MEMLEVEL, Z_DEFAULT_STRATEGY);
	int	status = deflate(str, flush);
	deflateEnd(str);

	compressedSize = str->total_out;
	delete str;
	return status == Z_STREAM_END;
}
bool gzipDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush)
{
	decompressedData = new char[decompressedSize];
	z_streamp str = new z_stream;
	initStream(str, compressedData, compressedSize, decompressedData, decompressedSize);

	inflateInit2(str, MAX_WBITS | Z_GZIP_HEADER);
	int status = inflate(str, flush);
	inflateEnd(str);

	delete str;
	return status == Z_STREAM_END;
}