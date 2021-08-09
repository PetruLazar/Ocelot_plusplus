#pragma once
#include "basic.h"

class Options
{
	static const char optionsFileName[];

	static ush _port;
	static int _max_players;
	static std::string _level_name, _motd;
	static sf::IpAddress _ip;

	//private constructor to disallow object of this class
	Options();
	~Options();

	//
	static Options options;

public:
	//thee are not included in server.properties
	static const std::string version;
	static bool allowJoin;

	static ush port();
	static int max_players();
	static const std::string & level_name();
	static const std::string& motd();
	static const sf::IpAddress& ip();
};