#include "region.h"
#include "../world.h"
#include "../types/error.h"
#include "../server/log.h"
#include <iostream>
#include "../types/basic.h"
#include "../mcexception.h"
#include <sstream>
#include <filesystem>

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
	uint fileSectionCount = fileSize >> 10;
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
	uint mapSize = map.size();
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
	if (end > map.size()) end = map.size();
	for (uint i = start; i < end; i++) if (map[i]) return false;
	return true;
}
void markZone(std::vector<bool>& map, uint start, uint end, bool occupied)
{
	if (end > map.size())
	{
		register uint oldSize = map.size();
		map.resize(end, occupied);
		end = oldSize; //only go from start to oldSize because the new elements are already set
	}
	for (uint i = start; i < end; i++) map[i] = occupied;
}

void Region::save(int relX, int relZ, bool autoFlush)
{
	//write the chunk to a buffer
	std::stringstream buffer;
	buffer.write("", 1); //writes 0 to the buffer - (chunk not compressed)
	chunks[relX][relZ]->write(buffer);
	std::string bufferContents = buffer.str();
	uint chunkSize = bufferContents.length();
	//transform the size from bytes to 1024B sections
	chunkSize = (chunkSize >> 10) + ((chunkSize & 0x3ff) != 0);

	//find a suitable location for the chunk
	HeaderEntry& headerEntry = regionHeader[relX][relZ];
	if (chunkSize <= headerEntry.length)
	{
		//new size is smaller than the old size

		//put the chunk in the same location
		regionFile.seekp((ull)headerEntry.fileOffset << 10);
		regionFile.write(bufferContents.c_str(), bufferContents.length());
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
		regionFile.write(bufferContents.c_str(), bufferContents.length());
		regionFile.seekp(((ull)relX << 5 | relZ) << 2);
		regionFile.write((char*)(&headerEntry), sizeof(HeaderEntry));
	}

	//flush is autoFlush is true
	if (autoFlush) regionFile.flush();

	//write the buffer to the region file
}
Chunk* Region::load(World* parent, int relX, int relZ)
{
	//try to read chunk from region file, if found
	ull offset = regionHeader[relX][relZ].fileOffset;
	//regionFile.seekg(offset);
	//regionFile.read((char*)(&offset), 4);
	if (!offset) return nullptr;
	regionFile.seekg(offset << 10);
	char isCompressed = 0;
	regionFile.read(&isCompressed, 1);
	//Byte sizeInKB;
	//regionFile.read((char*)(&sizeInKB), 1);
	std::istream* istr;
	//uint size;
	//regionFile.read((char*)(&size), 4);
	if (isCompressed)
	{
		throw "Chunk compression not supported yet.\n";
		//compressed: read, decompress and read from the resulted buffer
	}
	else
	{
		//not compressed: read directly from file
		istr = &regionFile;
	}

	//read chunk from buffer;
	Chunk* ch = new Chunk(parent->height);
	ch->read(*istr);
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
		std::cout << "\nIncorrect chunk unload at [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "]";
		throw runtimeWarning("Tried to unload a chunk the was not loaded");
	}
	chunk->loadCount--;
	IF_CHUNK_DEBUG(Log::txt() << "\nChunk [" << ((rX << 5) | relX) << ", " << ((rZ << 5) | relZ) << "] unloaded (" << chunk->loadCount << ")");
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
		Log::txt() << "\nIncorrect chunk set in region [ " << rX << ", " << rX << "], relative chunk [" << relX << ", " << relZ << "]";
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