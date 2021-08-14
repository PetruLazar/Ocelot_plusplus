#include "chunk.h"
#include "basic.h"

using namespace std;

/*void Chunk::loadChunk(nbt_compound& comp, int x, int z)
{
	int rx = x >> 5, rz = z >> 5;
	x &= 31; z &= 31;
	int tableIndex = ((x << 5) + z) << 2;
	fstream region("r." + to_string(rx) + '.' + to_string(rz) + ".mca", ios::binary | ios::in);
	buint fileOffset; char sectorSize;
	buint timestamp, size; compressionMethod meth{};

	region.seekg(tableIndex);
	fileOffset.read(region);
	sectorSize = fileOffset & 0xff;
	fileOffset >>= 8;
	fileOffset <<= 12;

	region.seekg(tableIndex + 4096);
	timestamp.read(region);

	region.seekg((uint)fileOffset);
	size.read(region);
	region.read((char*)&meth, sizeof(meth));
	char* data = new char[size],
		* res = new char[max_inflated_chunkSize];
	region.read(data, streamsize(size - 1));
	uint inflatedSize;
	if (!zlibDecompress(data, size, res, max_inflated_chunkSize, inflatedSize)) throw 145;
	if (!nbt::checkTag(res)) throw 0;
	comp.read(res);
}
void Chunk::updateChunk(nbt_compound& data)
{
	throw "WIP";
}*/