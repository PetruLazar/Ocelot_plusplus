#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include "types/bitarray.h"
#include "types/varData.h"
#include "json.h"
//#include "worldHeader.h"
#include "../zlib/include/zlib.h"
#include "nbt.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace std::filesystem;

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

bool zlibCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint& compressedSize, int level = 6, int flush = Z_FINISH)
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
bool zlibCompressNoAlloc(char* decompressedData, uint decompressedSize, char* compressedData, uint& compressedSize, int level = 6, int flush = Z_FINISH)
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
bool zlibDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush = Z_FINISH)
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
bool zlibDecompressNoAlloc(char* compressedData, uint compressedSize, char* decompressedData, uint decompressedSize, int flush = Z_FINISH)
{
	z_streamp str = new z_stream;
	initStream(str, compressedData, compressedSize, decompressedData, decompressedSize);

	inflateInit(str);
	int status = inflate(str, flush);
	inflateEnd(str);

	delete str;
	return status == Z_STREAM_END;
}

bool gzipCompress(char* decompressedData, uint decompressedSize, char*& compressedData, uint& compressedSize, int level = 6, int flush = Z_FINISH)
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
bool gzipDecompress(char* compressedData, uint compressedSize, char*& decompressedData, uint decompressedSize, int flush = Z_FINISH)
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

namespace Archive
{
	struct RegionHeaderEntry_v0_present
	{
		//both in 1024B sections
		ush fileOffset;
		ush length;
	};

	struct ChunkV0
	{
		struct Section
		{
			BiomesContainer biomes;
			BlockStatesContainer blockStates;

			bshort blockCount;
		};

		//arrays
		std::vector<LightSection> lightSections;

		//masks & empty masks
		BitArray* skyLightMask;
		BitArray* blockLightMask;
		BitArray* emptySkyLightMask;
		BitArray* emptyBlockLightMask;

		std::vector<Section> sections;

		BitArray* sectionMask;

		BitArray* heightmaps;

		inline ChunkV0(ull worldHeight)
		{
			//heightmap initialization
			heightmaps = new BitArray(256, bitCount(worldHeight));

			ull sectionCount = worldHeight >> 4;

			//section initialization
			sections.resize(sectionCount);

			//light section initialization
			lightSections.resize((ull)sectionCount + 2);
			skyLightMask = new BitArray(sectionCount + 2, 1);
			blockLightMask = new BitArray(sectionCount + 2, 1);
			emptySkyLightMask = new BitArray(sectionCount + 2, 1);
			emptyBlockLightMask = new BitArray(sectionCount + 2, 1);

			//section mask initialization
			sectionMask = new BitArray(sectionCount, 1);
		}

		void read(istream& file)
		{
			//chunk data

			//read sectionMask and heightmap
			sectionMask->read(file);
			heightmaps->read(file);

			for (Section& sec : sections)
			{
				sec.biomes.read(file);
				sec.blockStates.read(file);

				//read blockCount
				file.read((char*)(&sec.blockCount), 2);
			}

			//light data

			//read light masks
			skyLightMask->read(file);
			blockLightMask->read(file);
			emptySkyLightMask->read(file);
			emptyBlockLightMask->read(file);

			//read light data for each section
			for (uint i = 0; i < lightSections.size(); i++)
			{
				LightSection& sec = lightSections[i];
				sec.skyLight = new BitArray(4096, 4);
				if (skyLightMask->getElement(i))
				{
					//bit set in skyLightMask
					sec.skyLight->read(file);
				}
				else
				{
					//bit not set in skyLightMask
					if (emptySkyLightMask->getElement(i))
					{
						//bit set in emptySkyLightMask
						//section light has all zeros
					}
					else
					{
						//bit not set in emptySkyLightMask
						//should calculate light
					}
				}

				sec.blockLight = new BitArray(4096, 4);
				if (blockLightMask->getElement(i))
				{
					//bit set in blockLightMask
					sec.blockLight->read(file);
				}
				else
				{
					//bit not set in skyLightMask
					if (emptyBlockLightMask->getElement(i))
					{
						//bit set in emptyBlockLightMask
						//section light has all zeros
					}
					else
					{
						//bit not set in emptyBlockLightMask
						//should calculate light
					}
				}
			}
		}
		void write(ostream& file)
		{
			//chunk data

			//write sectionMask and heightmap
			sectionMask->write(file);
			heightmaps->write(file);

			//write each section
			for (Section& sec : sections)
			{
				sec.biomes.write(file);
				sec.blockStates.write(file);

				//write blockCount
				file.write((char*)(&sec.blockCount), 2);
			}

			//light data

			//write light masks
			skyLightMask->write(file);
			blockLightMask->write(file);
			emptySkyLightMask->write(file);
			emptyBlockLightMask->write(file);

			//write light data for each section
			for (uint i = 0; i < lightSections.size(); i++)
			{
				LightSection& sec = lightSections[i];

				if (skyLightMask->getElement(i))
				{
					sec.skyLight->write(file);
				}

				if (blockLightMask->getElement(i))
				{
					sec.blockLight->write(file);
				}
			}
		}
	};
}

void convertRegions0to1(const path& wldPath, ull worldHeight)
{
	//add chunk size field for unconpressed chunks, after "is compressed" byte
	//add number of block entities (uint 0)
	//add number of entities (uint 0)

	//read the world file to get the world height;

	Archive::RegionHeaderEntry_v0_present regHeader[32][32]{};
	char *chunks[32][32];

	//take each region file
	for (auto regFile : directory_iterator(wldPath / "regions"))
	{
		//init chunks matrix
		for (int x = 0; x < 32; x++) for (int y = 0; y < 32; y++) chunks[x][y] = nullptr;

		//prepare storage for converted chunks

		//read region header
		ifstream in(regFile.path(), ios::binary);
		in.read((char*)regHeader, sizeof(regHeader));
		//stringstream newReg(ios::out);

		//go through all the chunks
		for (int x = 0; x < 32; x++) for (int y = 0; y < 32; y++)
		{
			ull offset = regHeader[x][y].fileOffset;
			if (offset) //chunk is present
			{
				in.seekg(offset << 10);

				char isCompressed;

				in.read(&isCompressed, sizeof(isCompressed));

				if (isCompressed)
				{
					//chunk is compressed
					uint dataSizes[2]{};
					in.read((char*)dataSizes, sizeof(dataSizes));

					char *compressedData = new char[dataSizes[0]],
						*decompressedData = new char[dataSizes[1] + 8]; //leave space for the 2 null uints
					in.read(compressedData, dataSizes[0]);
					zlibDecompressNoAlloc(compressedData, dataSizes[0], decompressedData, dataSizes[1]);

					//modify chunk and compress it again
					for (uint i = 0; i < 8; i++)
						decompressedData[dataSizes[1] + i] = 0;
					dataSizes[1] += 8;
					delete[] compressedData;
					dataSizes[0] = dataSizes[1] + 9 + 10;//9 chunk header, 8 additional fields, 10 free space
					compressedData = new char[dataSizes[0]] + 9; 
					zlibCompressNoAlloc(decompressedData, dataSizes[1], compressedData, dataSizes[0]);
					compressedData -= 9;

					//put first three fields before the compressed data
					compressedData[0] = 1; //compressed
					*(uint*)(compressedData + 1) = dataSizes[0]; //compressedSize
					*(uint*)(compressedData + 5) = dataSizes[1]; //uncompressedSize

					chunks[x][y] = compressedData;
				}
				else
				{
					//chunk is not compressed
					streampos start = in.tellg();
					Archive::ChunkV0 *chunk = new Archive::ChunkV0(worldHeight);
					chunk->read(in);

					//modify chunk and leave it decompressed
					ull chunkLen = in.tellg() - start;
					char *chunkData = new char[chunkLen + 5 + 8]; //5 chunk header and 8 for additional data
					chunkData[0] = 0; //uncompressed
					*(uint*)(chunkData + 1) = uint(chunkLen + 8); //chunk size
					in.seekg((offset << 10) | 1);
					in.read(chunkData + 5, chunkLen);
					*(uint*)(chunkData + chunkLen + 5) = 0;
					*(uint*)(chunkData + chunkLen + 5 + 4) = 0;

					chunks[x][y] = chunkData;
				}
			}
		}

		//close region file
		in.close();

		//rewrite region file
		fstream nReg(regFile.path(), ios::binary | ios::in | ios::out);

		uint freeSections = 4; // sections 0-3 are the region header
		for (int x = 0; x < 32; x++) for (int y = 0; y < 32; y++)
		{
			if (chunks[x][y])
			{
				//chunk was converted

				char compressed = chunks[x][y][0];
				uint chunkLen = *(uint*)(chunks[x][y] + 1) + (compressed ? 9 : 5);
				uint sectionsOccupied = (chunkLen - 1) / 1024 + 1;

				regHeader[x][y].fileOffset = freeSections;
				regHeader[x][y].length = sectionsOccupied;

				nReg.seekg((ull)freeSections << 10);
				nReg.write(chunks[x][y], chunkLen);

				freeSections += sectionsOccupied;
			}
			else
			{
				//chunk was not found
				regHeader[x][y].fileOffset = 0;
				regHeader[x][y].length = 0;
			}
		}

		//update region header so that all existing chunks fit in the file
		nReg.seekg(0);
		nReg.write((char*)regHeader, sizeof(regHeader));

		nReg.close();
	}
}