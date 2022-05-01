#include "region.h"
#include "../world.h"
#include "../types/error.h"
#include "../server/log.h"
#include <iostream>
#include "../types/basic.h"
#include "../mcexception.h"
#include <sstream>
#include <filesystem>
#include "../server/options.h"

namespace fs = std::filesystem;

/*class chunkNotLoaded : public std::exception
{
	using std::exception::exception;
};*/

Region::Region(const mcString& worldName, int rX, int rZ) : rX(rX), rZ(rZ)
{
	//open the region file and read the header
	std::string regionFileName = "worlds/" + worldName + "/regions/r" + std::to_string(rX) + '.' + std::to_string(rZ) + ".bin";
	regionFile.open(regionFileName, std::ios::binary | std::ios::in | std::ios::out);
	if (regionFile.is_open())
	{
		regionFile.read((char*)regionHeader, sizeof(regionHeader));
	}
	else
	{
		{
			std::ofstream tmp(regionFileName, std::ios::binary);
			tmp.seekp(4095);
			tmp.write("", 1);
			tmp.close();
		}
		regionFile.open(regionFileName, std::ios::binary | std::ios::in | std::ios::out);
		if (!regionFile.is_open()) throw mcexception(mcexception::WORLD, "Cannot access or create region file", "world/region.cpp", __LINE__);
		for (uint i = 0; i < 512; i++) *((ull*)regionHeader + i) = 0;
	}
	//get the file size in bytes and in 1024B sections
	ull fileSize = regionFile.seekg(0, std::ios::_Seekend).tellg();
	uint fileSectionCount = (uint)(fileSize >> 10);
	if (fileSize & 0x3ff) fileSectionCount++;
	//building the file map
	regionFileMap.resize(fileSectionCount, false);
	//the first 4 sections is the header
	for (uint i = 0; i < 4; i++) regionFileMap[i] = true;
	for (uint cX = 0; cX < 32; cX++) for (uint cZ = 0; cZ < 32; cZ++) for (uint l = 0; l < regionHeader[cX][cZ].length; l++) regionFileMap[(ull)regionHeader[cX][cZ].fileOffset + l] = true;
}
Region::~Region()
{
	for (int z = 0; z < 32; z++) for (int x = 0; x < 32; x++) if (chunks[x][z])
	{
		//save(x, z);
		delete chunks[x][z];
	}
	regionFile.close();
}

uint getFreeZone(const std::vector<bool>& map, uint size)
{
	uint mapSize = (uint)map.size();
	uint len = 0, lastFreeZoneBegin = 4;
	for (uint i = 4; i < mapSize; i++) if (!map[i])
	{
		len++;
		if (len == size) return lastFreeZoneBegin;
	}
	else
	{
		lastFreeZoneBegin = i + 1;
		len = 0;
	}
	return lastFreeZoneBegin;
}
bool isFreeZone(const std::vector<bool>& map, uint start, uint end)
{
	if (end > map.size()) end = (uint)map.size();
	for (uint i = start; i < end; i++) if (map[i]) return false;
	return true;
}
void markZone(std::vector<bool>& map, uint start, uint end, bool occupied)
{
	if (end > map.size())
	{
		uint oldSize = (uint)map.size();
		map.resize(end, occupied);
		end = oldSize; //only go from start to oldSize because the new elements are already set
	}
	for (uint i = start; i < end; i++) map[i] = occupied;
}

/*void Region::writeChunkToFile(char* data, uint size, ull relX, ull relZ)
{
	uint chunkSize = (size >> 10) + ((size & 0x3ff) != 0);

	//find a suitable location for the chunk
	HeaderEntry& headerEntry = regionHeader[relX][relZ];
	if (chunkSize <= headerEntry.length)
	{
		//new size is smaller than the old size

		//put the chunk in the same location
		regionFile.seekp((ull)headerEntry.fileOffset << 10);
		regionFile.write(data, size);
		//free the sections in the map
		markZone(regionFileMap, chunkSize, headerEntry.length, false);
		//update the header in ram and file
		headerEntry.length = chunkSize;
		regionFile.seekp(((ull)relX << 5 | relZ) << 2);
		regionFile.write((char*)(&headerEntry), sizeof(HeaderEntry));
	}
	else
	{
		//new size is bigger tha the old size

		//update map and header entry

		//see if the chunk previosly existed and if the new chunk would fit in the same place (has enough 0s is the map after the current location
		if (headerEntry.length)
		{
			//chunk already exists in region file
			if (isFreeZone(regionFileMap, headerEntry.fileOffset + headerEntry.length, chunkSize))
			{
				//there is enough space at the same location
				markZone(regionFileMap, headerEntry.fileOffset + headerEntry.length, chunkSize, true);
				headerEntry.length = chunkSize;
				regionFile.seekp(headerEntry.fileOffset << 10);
			}
			else
			{
				//the chunk needs to be relocated
				markZone(regionFileMap, headerEntry.fileOffset, headerEntry.fileOffset + headerEntry.length, false);
				headerEntry.fileOffset = getFreeZone(regionFileMap, chunkSize);
				headerEntry.length = chunkSize;
				markZone(regionFileMap, headerEntry.fileOffset, headerEntry.fileOffset + chunkSize, true);
				regionFile.seekp(headerEntry.fileOffset << 10);
			}
		}
		else
		{
			//this chunk did not exist before
			headerEntry.fileOffset = getFreeZone(regionFileMap, chunkSize);
			headerEntry.length = chunkSize;
			markZone(regionFileMap, headerEntry.fileOffset, headerEntry.fileOffset + chunkSize, true);
			regionFile.seekp(headerEntry.fileOffset << 10);
		}
		regionFile.write(data, size);
		regionFile.seekp(((ull)relX << 5 | relZ) << 2);
		regionFile.write((char*)(&headerEntry), sizeof(HeaderEntry));
	}

	//flush is autoFlush is true
	if (autoFlush) regionFile.flush();
}*/
void Region::save(int relX, int relZ, bool autoFlush)
{
	//write the chunk to a buffer
	std::stringstream buffer;
	char* data;
	uint size;

	if (Options::chunkCompression())
	{
		chunks[relX][relZ]->write(buffer); //write the chunk to the buffer
		std::string bufferContents = buffer.str();

		//compress chunk
		char* decompressedData = (char*)bufferContents.c_str();
		uint decompressedSize = (uint)bufferContents.length();
		data = new char[9 + decompressedSize];
		size = decompressedSize;
		zlibCompressNoAlloc(decompressedData, decompressedSize, data + 9, size);
		//Log::

		//add the 1 for compressed, compressedSize and decompressedSize as the header
		*(data++) = 1;
		((uint*)data)[0] = size;
		((uint*)data--)[1] = decompressedSize;

		//add the size of the header to the total size
		size += 9;

		//delete the 
	}
	else
	{
		//leave chunk uncompressed
		buffer.write("", 1); // 0 for uncompressed
		chunks[relX][relZ]->write(buffer); //write the chunk to the buffer
		std::string bufferContents = buffer.str();

		//get data address and length
		data = (char*)bufferContents.c_str();
		size = (uint)bufferContents.length();
	}
	//transform the size from bytes to 1024B sections
	uint chunkSize = (size >> 10) + ((size & 0x3ff) != 0);

	//find a suitable location for the chunk
	HeaderEntry& headerEntry = regionHeader[relX][relZ];
	if (chunkSize <= headerEntry.length)
	{
		//new size is smaller than the old size

		//put the chunk in the same location
		regionFile.seekp((ull)headerEntry.fileOffset << 10);
		regionFile.write(data, size);
		//free the sections in the map
		markZone(regionFileMap, chunkSize, headerEntry.length, false);
		//update the header in ram and file
		headerEntry.length = chunkSize;
		regionFile.seekp(((ull)relX << 5 | relZ) << 2);
		regionFile.write((char*)(&headerEntry), sizeof(HeaderEntry));
	}
	else
	{
		//new size is bigger tha the old size

		//update map and header entry

		//see if the chunk previosly existed and if the new chunk would fit in the same place (has enough 0s is the map after the current location
		if (headerEntry.length)
		{
			//chunk already exists in region file
			if (isFreeZone(regionFileMap, headerEntry.fileOffset + headerEntry.length, chunkSize))
			{
				//there is enough space at the same location
				markZone(regionFileMap, headerEntry.fileOffset + headerEntry.length, chunkSize, true);
				headerEntry.length = chunkSize;
				regionFile.seekp((ull)headerEntry.fileOffset << 10);
			}
			else
			{
				//the chunk needs to be relocated
				markZone(regionFileMap, headerEntry.fileOffset, headerEntry.fileOffset + headerEntry.length, false);
				headerEntry.fileOffset = getFreeZone(regionFileMap, chunkSize);
				headerEntry.length = chunkSize;
				markZone(regionFileMap, headerEntry.fileOffset, headerEntry.fileOffset + chunkSize, true);
				regionFile.seekp((ull)headerEntry.fileOffset << 10);
			}
		}
		else
		{
			//this chunk did not exist before
			headerEntry.fileOffset = getFreeZone(regionFileMap, chunkSize);
			headerEntry.length = chunkSize;
			markZone(regionFileMap, headerEntry.fileOffset, headerEntry.fileOffset + chunkSize, true);
			regionFile.seekp((ull)headerEntry.fileOffset << 10);
		}
		regionFile.write(data, size);
		regionFile.seekp(((ull)relX << 5 | relZ) << 2);
		regionFile.write((char*)(&headerEntry), sizeof(HeaderEntry));
	}

	//flush is autoFlush is true
	if (autoFlush) regionFile.flush();

	//free any memory
	if (Options::chunkCompression()) delete[] data;
}
Chunk* Region::load(World* parent, int relX, int relZ)
{
	//try to read chunk from region file, if found
	ull offset = regionHeader[relX][relZ].fileOffset;
	if (!offset) return nullptr;
	regionFile.seekg(offset << 10);
	char isCompressed = 0;
	regionFile.read(&isCompressed, 1);

	if (isCompressed)
	{
		//compressed
		//read compressedSize, decompressedSize and compressedData
		uint dataSizes[2]{}; //[0] - compressedSize; [1] - decompressedSize
		regionFile.read((char*)dataSizes, sizeof(dataSizes));
		char* compressedData = new char[dataSizes[0]],
			*decompressedData;
		regionFile.read(compressedData, dataSizes[0]);
		//decompress data
		zlibDecompress(compressedData, dataSizes[0], decompressedData, dataSizes[1]);
		//read chunk
		Chunk* ch = new Chunk(parent->height);
		std::stringstream stream(std::string(decompressedData, dataSizes[1]));
		ch->read(stream);
		delete[] decompressedData;
		return ch;
	}
	//not compressed
	//read chunk from buffer;
	Chunk* ch = new Chunk(parent->height);
	ch->read(regionFile);
	return ch;

}
void Region::unload(World* parent)
{
	for (int z = 0; z < 32; z++) for (int x = 0; x < 32; x++) if (chunks[x][z]) unload(parent, x, z);
	loadedChunks = 0;
}
void Region::unload(World* parent, int relX, int relZ)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (!(chunk))
	{
		Log::info() << "Incorrect chunk unload at [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "]" << Log::endl;
		throw runtimeWarning("Tried to unload a chunk the was not loaded");
	}
	chunk->loadCount--;
	Log::debug(CHUNK_DEBUG) << "Chunk [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "] unloaded (" << chunk->loadCount << ")" << Log::endl;
	if (!chunk->loadCount)
	{
		//write the chunk to file
		//Log::txt() << "\nWriting chunk [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "] to file";
		save(relX, relZ);

		delete chunk;
		chunk = 0;
		loadedChunks--;
	}
}
void Region::set(int relX, int relZ, Chunk* p)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (chunk)
	{
		Log::info() << "Incorrect chunk set in region [ " << rX << ", " << rX << "], relative chunk [" << relX << ", " << relZ << "]" << Log::endl;
		//throw runtimeError("Tried to set a chunk when chunk was already set");
		return;
	}
	chunks[relX][relZ] = p;
	p->loadCount = 1;
	loadedChunks++;
}

Chunk* Region::get(World* parent, int relX, int relZ, bool increaseLoadCount)
{
	Chunk*& chunk = chunks[relX][relZ];
	if (chunk)
	{
		if (increaseLoadCount) chunk->loadCount++;
		return chunk;
	}
	//try to load it from file
	//Log::txt() << "\nLoading chunk [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "] from file";
	chunk = load(parent, relX, relZ);
	if (chunk)
	{
		//Log::txt() << "\n\tLoaded chunk";
		chunk->loadCount = 1;
		loadedChunks++;
	}
	//else Log::txt() << "\n\tDid not load chunk";
	return chunk;
}

bool Region::hasChunksLoaded() { return loadedChunks; }

BlockState& Region::getPaletteEntry(int relX, int relY, int relZ)
{
	Chunk* chunk = chunks[relX >> 4][relZ >> 4];
	if (!chunk) //throw chunkNotLoaded("Region::getPaletteEntry(int3): Chunk not loaded");
		throw mcexception(mcexception::WORLD, "Chunk not loaded", "world/region.cpp", __LINE__);
	return chunk->getPaletteEntry(relX & 0xf, relY, relZ & 0xf);
}
BlockState& Region::getPaletteEntry(int chunkX, int sectionY, int chunkZ, int paletteIndex)
{
	Chunk* chunk = chunks[chunkX][chunkZ];
	if (!chunk) throw mcexception(mcexception::WORLD, "Chunk not loaded", "world/region.cpp", __LINE__);
	return chunk->getPaletteEntry(sectionY, paletteIndex);
}
BlockState Region::getBlock(int relX, int relY, int relZ)
{
	Chunk* chunk = chunks[relX >> 4][relZ >> 4];
	if (!chunk) throw mcexception(mcexception::WORLD, "Chunk not loaded", "world/region.cpp", __LINE__);
	return chunk->getBlock(relX & 0xf, relY, relZ & 0xf);
}
void Region::setBlock(int relX, int relY, int relZ, const BlockState& bl)
{
	Chunk* chunk = chunks[relX >> 4][relZ >> 4];
	if (!chunk) throw mcexception(mcexception::WORLD, "Chunk not loaded", "world/region.cpp", __LINE__);
	chunk->setBlock(relX & 0xf, relY, relZ & 0xf, bl);
}