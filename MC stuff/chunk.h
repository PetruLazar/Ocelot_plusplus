#pragma once
#include "nbt/nbt.h"

class chunk
{

	enum class compressionMethod : byte
	{
		gzipCompression = 1,
		zlibCompression,
		none
	};

public:
	static const int max_inflated_chunkSize = 64 * 1024;
	static const int max_deflated_chunkSize = 128 * 1024;

	//load chunk into a compound tag
	static void loadChunk(nbt_compound&, int x, int z);
	static void updateChunk(nbt_compound&);
};

