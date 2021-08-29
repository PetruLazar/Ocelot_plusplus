#include "types.h"
#include "nbt.h"
//#include "particle.h"
#include "player.h"
#include "message.h"
#include "options.h"
#include "json.h"
#include "log.h"
#include "conio.h"
#include "world.h"

using namespace std;

const int mc_zlib_compression_level = 6;

int main()
{
	cout << "Starting server...";
	srand((uint)time(nullptr));
	log::initialize();
	World::loadAll();
	sf::TcpSocket* buffer = new sf::TcpSocket;
	sf::TcpListener listener;

	if (listener.listen(Options::port(), Options::ip()) != sockStat::Done)
	{
		system("pause");
		return 0;
	}
	cout << "\nServer started on " << Options::ip() << ':' << Options::port();
	listener.setBlocking(false);
	bool keepAlive = true;
	while (keepAlive)
	{
		clock_t time = clock();
		//accept connections
		if (listener.accept(*buffer) == sockStat::Done)
		{
			Player::players.push_back(new Player(buffer));
			cout << '\n' << buffer->getRemoteAddress() << ':' << buffer->getRemotePort() << " connected.";
			buffer = new sf::TcpSocket;
		}

		//receive messages
		for (int64 i = 0; i < (int64)Player::players.size(); i++) try
		{
			Player::players[i]->updateNet(time);
		}
		catch (runtimeError obj)
		{
			cout << "\nRuntime error: " << obj.msg;
		}
		catch (runtimeWarning obj)
		{
			cout << "\nRuntime warning: " << obj.msg;
		}
		catch (protocolError obj)
		{
			cout << "\nProtocol error: " << obj.msg;
		}
		catch (protocolWarning obj)
		{
			log::txt() << "\nProtocol warning: " << obj.msg;
		}
		catch (const char* err_msg)
		{
			cout << "\nError (old format): " << err_msg;
		}
		catch (...)
		{
			cout << "\nUnknown error.";
		}
		Player::clearDisconnectedPlayers();
		//exit on excape - makes checking for memory leaks with _CrtDumpMemoryLeaks() possible - comment the next line if needed
		if (_kbhit())
		{
			//27 is escape
			int key = _getch();
			if (key == 27) break;
			if (key == 'w')
			{
				World::spawnWorld = -World::spawnWorld + 1;
				if (World::spawnWorld) cout << "\nPlayers now spawn in world 1";
				else cout << "\nPlayers now spawn in world 0";
			}
		}
	}

	for (int64 i = 0; i < (int64)Player::players.size(); i++) try
	{
		message::play::send::disconnect(Player::players[i], Chat("Server closed."));
		delete Player::players[i];
	}
	catch (runtimeError obj)
	{
		cout << "\nRuntime error: " << obj.msg;
	}
	catch (runtimeWarning obj)
	{
		cout << "\nRuntime warning: " << obj.msg;
	}
	catch (protocolError obj)
	{
		cout << "\nProtocol error: " << obj.msg;
	}
	catch (protocolWarning obj)
	{
		cout << "\nProtocol warning: " << obj.msg;
	}
	catch (const char* err_msg)
	{
		cout << "\nError: " << err_msg;
	}
	catch (...)
	{
		cout << "\nUnknown error.";
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

	delete buffer;
	try
	{
		World::unloadAll();
	}
	catch (runtimeError obj)
	{
		cout << "\nRuntime error: " << obj.msg;
	}
	catch (runtimeWarning obj)
	{
		cout << "\nRuntime warning: " << obj.msg;
	}
	catch (protocolError obj)
	{
		cout << "\nProtocol error: " << obj.msg;
	}
	catch (protocolWarning obj)
	{
		cout << "\nProtocol warning: " << obj.msg;
	}

	return 0;
}

class MemoryLeakDetection
{
public:
	~MemoryLeakDetection()
	{
		_CrtDumpMemoryLeaks();
	}
} memoryLeakDetection;