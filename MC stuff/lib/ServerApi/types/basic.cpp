#include "basic.h"

clock_t cycleTime;

/*#define Z_DEFAULT_MEMLEVEL 8
#define Z_GZIP_HEADER 16

void initStream(z_streamp str, char* src, uint srcSize, char* dest, uint destSize)
{
	str->opaque = Z_NULL;
	str->zalloc = Z_NULL;
	str->zfree = Z_NULL;

	str->next_in = (byte*)src;
	str->avail_in = srcSize;
	str->next_out = (byte*)dest;
	str->avail_out = destSize;
}

bool zlibCompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int level, int flush)
{
	z_streamp str = new z_stream;
	initStream(str, src, srcSize, dest, destSize);

	deflateInit(str, level);
	int status = deflate(str, flush);
	deflateEnd(str);

	trueSize = uint((char*)str->next_out - dest);
	delete str;
	return status == Z_STREAM_END;
}
bool zlibDecompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int flush)
{
	z_streamp str = new z_stream;
	initStream(str, src, srcSize, dest, destSize);

	inflateInit(str);
	int status = inflate(str, flush);
	inflateEnd(str);

	trueSize = uint((char*)str->next_out - dest);
	delete str;
	return status == Z_STREAM_END;
}

bool gzipCompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int level, int flush)
{
	z_streamp str = new z_stream;
	initStream(str, src, srcSize, dest, destSize);

	deflateInit2(str, level, Z_DEFLATED, 15 | Z_GZIP_HEADER, Z_DEFAULT_MEMLEVEL, Z_DEFAULT_STRATEGY);
	int	status = deflate(str, flush);
	deflateEnd(str);

	trueSize = str->total_out;
	delete str;
	return status == Z_STREAM_END;
}
bool gzipDecompress(char* src, uint srcSize, char* dest, uint destSize, uint& trueSize, int flush)
{
	z_streamp str = new z_stream;
	initStream(str, src, srcSize, dest, destSize);

	int status0 = inflateInit2(str, MAX_WBITS | Z_GZIP_HEADER);
	int status = inflate(str, Z_NO_FLUSH);
	int status2 = inflateEnd(str);

	trueSize = str->total_out;
	delete str;
	return status == Z_STREAM_END;
}*/