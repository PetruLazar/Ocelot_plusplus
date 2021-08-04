#include "endian.h"
#include "varData.h"
#include "nbt.h"
#include "chunk.h"
#include "position.h"
#include "mcString.h"
#include "fixedPoint.h"
//#include "particle.h"
#include "player.h"
#include "message.h"
#include "options.h"
#include "json/json.h"
#include "log.h"
#include "conio.h"

using namespace std;

const int mc_zlib_compression_level = 6;

int main()
{
	log::initialize();
	srand((uint)time(nullptr));
	sf::TcpSocket* buffer = new sf::TcpSocket;
	sf::TcpListener listener;

	if (listener.listen(Options::port()) != sockStat::Done)
	{
		system("pause");
		return 0;
	}
	cout << "Server started.";
	listener.setBlocking(false);
	bool keepAlive = true;
	while (keepAlive)
	{
		//accept connections
		if (listener.accept(*buffer) == sockStat::Done)
		{
			if (Player::players.size() == 1)
			{
				//refuse connection - debugging purposes
				buffer->disconnect();
				cout << "\nConnection refused.";
			}
			else
			{
				//accept connection
				Player::players.push_back(new Player(buffer));
				buffer = new sf::TcpSocket;
				cout << "\nConnection accepted.";
			}
		}

		//receive messages
		for (int64 i = 0; i < (int64)Player::players.size(); i++) try
		{
			Player::players[i]->updateNet();
		}
		catch (const char* err_msg)
		{
			cout << "\nError: " << err_msg;
		}
		catch (...)
		{
			cout << "\nUnknown error.";
		}
		Player::clearDisconnectedPlayers();
		//makes checking for memory leaks with _CrtDumpMemoryLeaks() possible - comment the next line if needed
		if (_kbhit()) break;
	}

	//fill an existing chunk with stone
	/*try
	{
		int cX, cZ, rX, rZ;
		cout << "Chunk X and Z: "; cin >> cX >> cZ;
		rX = cX >> 5; rZ = cZ >> 5;

		nbt_compound chunkNbt;
		chunk::loadChunk(chunkNbt, cX, cZ);

		//modifying chunk
		try
		{
			for (uint i = 1; i <= 16; i++)
			{
				nbt& section = chunkNbt["Level"].vTag("Sections").vTag(i),
					& palette = section.vTag("Palette"),
					& states = section.vTag("BlockStates");

				palette.resize(1);
				palette.vTag(0).vTag("Name").vString() = "minecraft:stone";

				states.resize(256);
				for (uint i = 0; i < 256; i++) states.vLong(i) = 0;
			}
		}
		catch (...)
		{
			throw 5;
		}

		//compressing chunk data
		char* modifiedChunkData = new char[chunk::max_inflated_chunkSize];
		char* compressedData = new char[chunk::max_deflated_chunkSize];
		char* modifiedChunkDataEnd = modifiedChunkData;
		chunkNbt.write(modifiedChunkDataEnd);

		z_stream zstr{};
		zstr.opaque = Z_NULL;
		zstr.zalloc = Z_NULL;
		zstr.zfree = Z_NULL;

		zstr.next_in = (byte*)modifiedChunkData;
		zstr.avail_in = (uint)(modifiedChunkDataEnd - modifiedChunkData);
		zstr.next_out = (byte*)compressedData;
		zstr.avail_out = chunk::max_deflated_chunkSize;

		deflateInit(&zstr, mc_zlib_compression_level);
		int status = deflate(&zstr, 4);
		deflateEnd(&zstr);

		if (status != Z_STREAM_END) throw 6;

		//update chunk in region file
		fstream regFileOut("r." + to_string(rX) + '.' + to_string(rZ) + ".mca", ios::binary | ios::ate | ios::in | ios::out);
		bint newSize = (int)((char*)zstr.next_out - compressedData + 1);
		byte _4kSize = (newSize - 1) / 4096 + 1;

		buint offset;
		regFileOut.seekp((cX & 31 << 5 | cZ & 31) << 2);
		char* s = new char[4];
		regFileOut.read(s, 4);
		offset.read(s);
		offset >>= 8;
		offset <<= 12;
		regFileOut.seekp((uint)offset);
		newSize.write(regFileOut);
		regFileOut.write("\002", 1);
		regFileOut.write(compressedData, (streamsize)newSize - 1);

		regFileOut.close();

		delete[] modifiedChunkData;
		delete[] compressedData;
	}
	catch (int e)
	{
		switch (e)
		{
		case 1:
			cout << "Chunk not present";
			break;
		case 2:
			cout << "Chunk not compressed with zlib";
			break;
		case 3:
			cout << "Error occured while inflating";
			break;
		case 4:
			cout << "Invalid tag";
			break;
		case 5:
			cout << "Error occured while modifying chunk data";
			break;
		case 6:
			cout << "Error occured during compression";
			break;
		}
	}*/

	//example of forming a tag in memory, writing in file, reading from file, and converting in a string
	/*{
		ofstream out("test.bin", ios::binary);

		vector <nbt*> alltags[5];
		for (int i = 0; i < 5; i++)
		{
			alltags[i].push_back(new nbt_byte(55, "byteTag"));
			alltags[i].push_back(new nbt_short(-15642, "shortTag"));
			alltags[i].push_back(new nbt_int(-1265102002, "intTag"));
			alltags[i].push_back(new nbt_long(156202486155401, "longTag"));
			alltags[i].push_back(new nbt_float(1.75f, "floatTag"));
			alltags[i].push_back(new nbt_double(125.1569, "doubleTag"));
			alltags[i].push_back(new nbt_byte_array(new char[5]{ 16,-98,12,64,-5 }, 5, "byteArrayTag"));
			//alltags[i].push_back(new nbt_string(1.75f, "height"));
			alltags[i].push_back(new nbt_int_array(new int[5]{ 49,15264,-64521,18,-84 }, 5, "intArrayTag"));
			alltags[i].push_back(new nbt_long_array(new int64[5]{ -156497635214,12345678910,9876543210123,-597846325448,3 }, 5, "longArrayTag"));
		}

		alltags[0].push_back(new nbt_compound(alltags[1], "compoundPart"));
		alltags[0].push_back(new nbt_list(new nbt * [3]{ new nbt_compound(alltags[2]),new nbt_compound(alltags[3]),new nbt_compound(alltags[4]) }, 3, "listTag"));

		nbt_compound test(alltags[0], "finalCompTag"), test2;
		test.write(out);

		out.close();

		ifstream in("test.bin", ios::binary);

		if (!nbt::checkFileForTag(in))
		{
			cout << "File does not contain the right tag.\n";
			in.close();
			return 0;
		}
		test2.read(in);
		//cout << test2["mass"].getStringValue() << ' ' << test2["age"].getStringValue() << ' ' << test2["height"].getStringValue();

		in.close();

		ofstream debugfile("log.txt");

		debugfile << test.to_string() << '\n' << test2.to_string();

		debugfile.close();
	}*/

	delete buffer;

	//triggers some false alarms: check with DEBUG_ALLOCATIONS (basic.h) defined to validate output
	_CrtDumpMemoryLeaks();

	return 0;
}