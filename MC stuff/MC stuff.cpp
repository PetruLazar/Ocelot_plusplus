#include "player/player.h"
#include "server/options.h"
#include "server/log.h"
#include "types/error.h"
#include "player/message.h"
#include <iostream>
#include "types/chat.h"
#include <server/console.h>
#include <conio.h>
#include <SFML/Network/TcpListener.hpp>
#include <Windows.h>
#include <world/noise.h>
#include <debug.cpp>
#include <filesystem>
#include "types/registry.h"

namespace fs = std::filesystem;
using namespace std;

const int mc_zlib_compression_level = 6;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdLineShow)
{
	Log::initialize();
	ServerConsole::AllocCosole();

	//rand seeding
	srand((uint)time(nullptr));

	//listen on port
	sf::TcpListener listener;
	if (listener.listen(Options::port(), Options::ip()) != sockStat::Done)
	{
		system("pause");
		return 0;
	}
	Log::txt() << "Starting server on " << Options::ip().toString() << ':' << Options::port();
	listener.setBlocking(false);

	//loading registries...
	Log::txt() << "\nLoading registries...";
	Registry::loadRegistry();

	//loading vanilla tags
	Log::txt() << "\nLoading vanilla tags...";
	TagGroup::loadVanillaTags();

	Log::txt() << "\nLoading worlds...";
	World::loadAll();
	sf::TcpSocket* buffer = new sf::TcpSocket;
	Log::txt() << "\nLoad complete.";

	//main loop
	bool keepAlive = true;
	while (keepAlive)
	{
		cycleTime = clock();
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
			Player::players[i]->updateNet();
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
			Log::txt() << "\nProtocol warning: " << obj.msg;
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

	cout << "\nKicking players...";
	for (int64 i = 0; i < (int64)Player::players.size(); i++) try
	{
		message::play::send::disconnect(Player::players[i], Chat("Server closed."));
		//Player::players[i]->disconnect();
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
		Log::txt() << "\nProtocol warning: " << obj.msg;
	}
	catch (const char* err_msg)
	{
		cout << "\nError: " << err_msg;
	}
	catch (...)
	{
		cout << "\nUnknown error.";
	}

	for (int64 i = 0; i < (int64)Player::players.size(); i++)
	{
		while (Player::players[i]->Connected()) Player::players[i]->updateNet();
		delete Player::players[i];
	}

	delete buffer;
	try
	{
		cout << "\nUnloading worlds...";
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

	ServerConsole::FreeConsole();
	return 0;
}