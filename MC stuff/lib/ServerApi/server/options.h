#pragma once
#include "SFML/Network/IpAddress.hpp"
#include "../types/typedefs.h"
#include "../types/apidef.h"
#include "../json.h"

class Options
{
	static const char optionsFileName[];

	static ush _port;
	static int _max_players;
	static std::string _mainWorldName, _motd;
	static sf::IpAddress _ip;
	static Byte _viewDistance;
	static Byte _simulationDistance;
	static bool _chunkCompression;
	static short _networkCompression;

	//private constructor to disallow object of this class
	Options();
	~Options();

	static bool loaded;

public:
	SERVER_API static void Load();
	SERVER_API static void Unload();

	//these are not included in server.properties
	SERVER_API static int currentProtocol();
	SERVER_API static const std::string& currentVersion();
	//SERVER_API static const std::string version;
	SERVER_API static const json_compound version;
	SERVER_API static bool allowJoin;

	SERVER_API static ush port();
	SERVER_API static int max_players();
	SERVER_API static const std::string& mainWorldName();
	SERVER_API static const std::string& motd();
	SERVER_API static const sf::IpAddress& ip();
	SERVER_API static Byte viewDistance();
	SERVER_API static Byte simulationDistance();
	SERVER_API static bool chunkCompression();
	SERVER_API static short networkCompression();
};