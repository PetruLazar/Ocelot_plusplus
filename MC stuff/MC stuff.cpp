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
#include "player/command.h"

namespace fs = std::filesystem;
using namespace std;

const int mc_zlib_compression_level = 6;
/*
class Test1
{
public:
	Test1()
	{
		cout << "\ntest1 constructed.";
	}
	~Test1()
	{
		cout << "\ntest1 destroyed.";
	}
} test1;

class Test2
{
public:
	Test2()
	{
		cout << "\ntest2 constructed.";
	}
	~Test2()
	{
		cout << "\ntest2 destroyed.";
	}
} test2;
*/

bool keepServerOpen = true;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdLineShow)
{
	Log::initialize();
	ServerConsole::AllocConsole();

	//rand seeding
	srand((uint)time(nullptr));

	//listen on port
	sf::TcpListener listener;
	if (listener.listen(Options::port(), Options::ip()) != sockStat::Done)
	{
		system("pause");
		ServerConsole::FreeConsole();
		return 0;
	}
	Log::txt() << "Starting server on " << Options::ip() << ':' << Options::port() << Log::flush;
	listener.setBlocking(false);

	//loading registries...
	Log::txt() << "\nLoading registries..." << Log::flush;
	Registry::loadRegistriesAndPalette();

	//loading vanilla tags
	Log::txt() << "\nLoading vanilla tags..." << Log::flush;
	TagGroup::loadVanillaTags();

	Log::txt() << "\nLoading worlds..." << Log::flush;
	if (!World::loadAll())
	{
		Log::txt() << "\nError: Spawn world \"" << Options::mainWorldName() << "\" not found." << Log::flush;
		system("pause");
		ServerConsole::FreeConsole();
		return 0;
	}
	sf::TcpSocket* buffer = new sf::TcpSocket;
	Log::txt() << "\nLoad complete." << Log::flush;

	//testing...
	//cout << '\n' << World::worlds[1]->get(0, 0)->getBlock(0, 80, 0).id << '\n';

	//main loop
	while (keepServerOpen)
	{
		cycleTime = clock();
		//accept connections
		if (listener.accept(*buffer) == sockStat::Done)
		{
			Player::players.push_back(new Player(buffer));
			IF_LOG_ALL_CONNECTIONS(Log::txt() << '\n' << buffer->getRemoteAddress() << ':' << buffer->getRemotePort() << " connected.");
			buffer = new sf::TcpSocket;
		}

		//receive messages
		for (int64 i = 0; i < (int64)Player::players.size(); i++) try
		{
			Player::players[i]->updateNet();
		}
		catch (runtimeError obj)
		{
			Log::txt() << "\nRuntime error: " << obj.msg;
		}
		catch (runtimeWarning obj)
		{
			Log::txt() << "\nRuntime warning: " << obj.msg;
		}
		catch (protocolError obj)
		{
			Log::txt() << "\nProtocol error: " << obj.msg;
		}
		catch (protocolWarning obj)
		{
			Log::txt() << "\nProtocol warning: " << obj.msg;
		}
		catch (const char* err_msg)
		{
			Log::txt() << "\nError (old format): " << err_msg;
		}
		catch (...)
		{
			Log::txt() << "\nUnknown error.";
		}
		Player::clearDisconnectedPlayers();

		//exit on escape - makes checking for memory leaks with _CrtDumpMemoryLeaks() possible - comment the next line if needed
		if (_kbhit())
		{
			switch (_getch())
			{
			case 27: //escape
				keepServerOpen = false;
				break;
			}
		}

		Log::Flush();
	}

	Log::txt() << "\nKicking players...";
	for (int64 i = 0; i < (int64)Player::players.size(); i++) try
	{
		message::play::send::disconnect(Player::players[i], Chat("Server closed."));
		//Player::players[i]->disconnect();
	}
	catch (runtimeError obj)
	{
		Log::txt() << "\nRuntime error: " << obj.msg;
	}
	catch (runtimeWarning obj)
	{
		Log::txt() << "\nRuntime warning: " << obj.msg;
	}
	catch (protocolError obj)
	{
		Log::txt() << "\nProtocol error: " << obj.msg;
	}
	catch (protocolWarning obj)
	{
		Log::txt() << "\nProtocol warning: " << obj.msg;
	}
	catch (const char* err_msg)
	{
		Log::txt() << "\nError: " << err_msg;
	}
	catch (...)
	{
		Log::txt() << "\nUnknown error.";
	}

	for (int64 i = 0; i < (int64)Player::players.size(); i++)
	{
		while (Player::players[i]->Connected()) Player::players[i]->updateNet();
		delete Player::players[i];
	}

	delete buffer;
	try
	{
		Log::txt() << "\nUnloading worlds...";
		World::unloadAll();
	}
	catch (runtimeError obj)
	{
		Log::txt() << "\nRuntime error: " << obj.msg;
	}
	catch (runtimeWarning obj)
	{
		Log::txt() << "\nRuntime warning: " << obj.msg;
	}
	catch (protocolError obj)
	{
		Log::txt() << "\nProtocol error: " << obj.msg;
	}
	catch (protocolWarning obj)
	{
		Log::txt() << "\nProtocol warning: " << obj.msg;
	}

	ServerConsole::FreeConsole();
	Registry::unloadRegistriesAndPalette();
	return 0;
}