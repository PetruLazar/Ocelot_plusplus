
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

bool keepServerOpen = true;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdLineShow)
{
	Log::initialize();
	Server::AllocConsole();

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

	Log::info() << "Server started! " << Log::Bench("server") << Log::flush;

	//main loop
	while (keepServerOpen)
	{
		cycleTime = clock();
		//accept connections
		if (listener.accept(*buffer) == sockStat::Done)
		{
			Player::players.emplace_back(new Player(buffer));
			Log::debug(PROTOCOL_WARNINGS) << buffer->getRemoteAddress() << ':' << buffer->getRemotePort() << " connected.\n";
			buffer = new sf::TcpSocket;
		}

		//receive messages
		for (int64 i = 0; i < (int64)Player::players.size(); i++) try
		{
			Player::players[i]->updateNet();
		}
		catch (const mcException& e)
		{
			Log::error() << e.what() << Log::endl;
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

	Log::info() << "Kicking players..." << Log::endl;
	for (int64 i = 0; i < (int64)Player::players.size(); i++) try
	{
		message::play::send::disconnect(Player::players[i], Chat("Server closed."));
		//Player::players[i]->disconnect();
	}
	catch (const mcException& e)
	{
		Log::error() << e.what() << Log::endl;
	}

	for (int64 i = 0; i < (int64)Player::players.size(); i++)
	{
		while (Player::players[i]->Connected()) Player::players[i]->updateNet();
		delete Player::players[i];
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

	Registry::unloadRegistriesAndPalette();
	recipe::Manager::unloadRecipes();
	Server::FreeConsole();
	return 0;
}