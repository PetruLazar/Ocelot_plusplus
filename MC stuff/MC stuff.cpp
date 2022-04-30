#include "player/player.h"
#include "server/options.h"
#include "server/log.h"
#include "types/error.h"
#include "player/message.h"
#include <iostream>
#include "types/chat.h"
#include <server/server.h>
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
		catch (runtimeError obj)
		{
			Log::error() << "Runtime error: " << obj.msg << Log::endl;
		}
		catch (runtimeWarning obj)
		{
			Log::warn() << "Runtime warning: " << obj.msg << Log::endl;
		}
		catch (protocolError obj)
		{
			Log::error() << "Protocol error: " << obj.msg << Log::endl;
		}
		catch (protocolWarning obj)
		{
			Log::warn() << "Protocol warning: " << obj.msg << Log::endl;
		}
		catch (const char* err_msg)
		{
			Log::error() << "Error (old format): " << err_msg << Log::endl;
		}
		catch (const std::exception& e)
		{
			Log::error() << "Exception thrown: " << e.what() << Log::endl;
		}
		catch (...)
		{
			Log::error() << "Unknown error." << Log::endl;
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
	catch (runtimeError obj)
	{
		Log::error() << "Runtime error: " << obj.msg << Log::endl;
	}
	catch (runtimeWarning obj)
	{
		Log::warn() << "Runtime warning: " << obj.msg << Log::endl;
	}
	catch (protocolError obj)
	{
		Log::error() << "Protocol error: " << obj.msg << Log::endl;
	}
	catch (protocolWarning obj)
	{
		Log::warn() << "Protocol warning: " << obj.msg << Log::endl;
	}
	catch (const char* err_msg)
	{
		Log::error() << "Error: " << err_msg << Log::endl;
	}
	catch (...)
	{
		Log::error() << "Unknown error." << Log::endl;
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
	catch (runtimeError obj)
	{
		Log::error() << "Runtime error: " << obj.msg << Log::endl;
	}
	catch (runtimeWarning obj)
	{
		Log::warn() << "Runtime warning: " << obj.msg << Log::endl;
	}
	catch (protocolError obj)
	{
		Log::error() << "Protocol error: " << obj.msg << Log::endl;
	}
	catch (protocolWarning obj)
	{
		Log::warn() << "Protocol warning: " << obj.msg << Log::endl;
	}

	Server::FreeConsole();
	Registry::unloadRegistriesAndPalette();
	recipe::Manager::unloadRecipes();
	return 0;
}