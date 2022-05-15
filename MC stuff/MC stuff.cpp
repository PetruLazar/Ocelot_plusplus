
#include <iostream>
#include <filesystem>
#include <conio.h>
#include <Windows.h>

#include <SFML/Network/TcpListener.hpp>

#include "debug/debug.h"
#include "debug/log.h"
#include "debug/mcexceptions.h"

#include "player/player.h"
#include "server/options.h"

#include "server/server.h"
#include "types/registry.h"
#include "types/chat.h"
#include "player/command.h"
#include "player/message.h"
#include "world/noise.h"

namespace fs = std::filesystem;
using namespace std;

const int mc_zlib_compression_level = 6;

#define SHOW_TICKRATE false

#if SHOW_TICKRATE == true

ull ticks = 0;
clock_t lastsec = 0;

void DisplayTicksPerSec()
{
	if (cycleTime - lastsec >= 1000)
	{
		lastsec = cycleTime;

		Log::info() << "Ticks per second: " << ticks << Log::endl;
		ticks = 0;
	}
}
#define incTicks ticks++

#else

#define DisplayTicksPerSec()
#define incTicks

#endif

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
{
	Log::initialize();
	Server::AllocConsole();
	if (!Options::Load())
	{
		Log::error() << "Failed to load server.properties." << Log::flush;
		system("pause");
		Server::FreeConsole();
		return 0;
	}

	//rand seeding
	srand((uint)time(nullptr));

	//listen on port
	sf::TcpListener listener;
	if (listener.listen(Options::port(), Options::ip()) != sockStat::Done)
	{
		system("pause");
		Server::FreeConsole();
		return 0;
	}
	Log::Bench("server");
	Log::info() << "Starting server on " << Options::ip() << ':' << Options::port() << "..." << Log::flush;
	listener.setBlocking(false);

	//loading registries...
	Log::Bench("registries");
	Log::info() << "Loading registries... ";
	Registry::loadRegistriesAndPalette();
	Log::none() << Log::Bench("registries") << Log::flush;

	//loading vanilla tags
	Log::Bench("tags");
	Log::info() << "Loading vanilla tags... ";
	TagGroup::loadVanillaTags();
	Log::none() << Log::Bench("tags") << Log::flush;

	//loading recipies
	Log::Bench("recipies");
	Log::info() << "Loading vanilla recipies... ";
	recipe::Manager::loadRecipes();
	Log::none() << Log::Bench("recipies") << Log::flush;

	Log::Bench("worlds");
	Log::info() << "Loading worlds..." << Log::flush;
	if (!World::loadAll())
	{
		Log::error() << "Error: Spawn world \"" << Options::mainWorldName() << "\" not found." << Log::flush;
		Log::warn() << "Shutting down..." << Log::flush;
		World::unloadAll();
		Registry::unloadRegistriesAndPalette();
		Server::FreeConsole();
		return 0;
	}

	sf::TcpSocket* buffer = new sf::TcpSocket;
	clock_t lasttick = clock();

	Log::info() << "Server started! " << Log::Bench("server") << Log::flush;

	//main loop
	while (Server::keepServerOpen)
	{
		cycleTime = clock();
		//accept connections
		if (listener.accept(*buffer) == sockStat::Done)
		{
			Player::players.emplace_front(new Player(buffer));
			Log::debug(PROTOCOL_WARNINGS) << buffer->getRemoteAddress() << ':' << buffer->getRemotePort() << " connected.\n";
			buffer = new sf::TcpSocket;
		}

		//receive and send messages
		Player::updateAll();

		//check if it's time for a tick
		if (cycleTime - lasttick >= 50)
		{
			lasttick += 50; //this way, the ticking catches up and remains in sync with time

			//tick
			incTicks;
		}

		DisplayTicksPerSec();

		//exit on escape - makes checking for memory leaks with _CrtDumpMemoryLeaks() possible - comment the next line if needed
		if (_kbhit())
		{
			switch (_getch())
			{
			case 27: //escape
				Server::keepServerOpen = false;
				break;
			}
		}

		Log::Flush();
	}
	listener.close();

	Log::info() << "Kicking players..." << Log::endl;
	for (Player* p : Player::players) try
	{
		message::play::send::disconnect(p, Chat("Server closed."));
		//Player::players[i]->disconnect();
	}
	catch (const mcException& e)
	{
		Log::error() << e.what() << Log::endl;
	}

	for (Player* p : Player::players)
	{
		while (p->Connected()) p->updateNet();
		delete p;
	}

	delete buffer;
	try
	{
		Log::info() << "Unloading worlds..." << Log::endl;
		World::unloadAll();
	}
	catch (const mcException& e)
	{
		Log::error() << e.what() << Log::endl;
	}

	Options::Unload();
	Registry::unloadRegistriesAndPalette();
	recipe::Manager::unloadRecipes();
	Server::FreeConsole();

	if (Server::restartOnClose)
	{
		system("start \"\" \"MC stuff.exe\"");
	}

	return 0;
}