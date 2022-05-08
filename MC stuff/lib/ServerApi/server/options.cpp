
#include <iostream>
#include <fstream>
#include "../player/player.h"
#include "../mcexception.h"

#include "options.h"

#include "../debug/log.h"

using namespace std;

//const char* elemNotFound = "Setting not found.";

const char commentChar = '#';

const char Options::optionsFileName[] = "server.properties";
ush Options::_port = 25565;
int Options::_max_players = 100;
std::string Options::_mainWorldName = "world", Options::_motd = "{\"text\":\"A Minecraft server.\"}";
sf::IpAddress Options::_ip = sf::IpAddress::Any;
Byte Options::_viewDistance = 10;
Byte Options::_simulationDistance = 10;
bool Options::_chunkCompression = true;
short Options::_networkCompression = 128;

bool Options::loaded = false;

//const string Options::version = "\"version\":{\"name\":\"1.18.2\",\"protocol\":758}";
const json_compound Options::version("version", new json*[2]{
		new json_string("name", "1.18.2"),
		new json_int("protocol", 758)
	}, 2);
bool Options::allowJoin = true;
int Options::currentProtocol() { return version["protocol"].iValue(); }
const string& Options::currentVersion() { return version["version"].value(); }

ull parseUnsigned(const string& value)
{
	size_t check;
	ull v = stoull(value, &check);
	if (check != value.length()) throw invalid_argument("");
	return v;
}
int64 parse(const string& value)
{
	size_t check;
	int64 v = stoll(value, &check);
	if (check != value.length()) throw invalid_argument("");
	return v;
}
ush parseUShort(const string& value)
{
	ull v = parseUnsigned(value);
	if (v > 0xffff) throw out_of_range("");
	return (ush)v;
}
int parseInt(const string& value)
{
	size_t check;
	int v = stoi(value,&check);
	if (check != value.length()) throw invalid_argument("");
	return v;
}
short parseShort(const string& value)
{
	int v = parseInt(value);
	if (v > 0x7fff || v < 0xffff8000i32) throw out_of_range("");

	return (int)v;
}
Byte parseByte(const string& value)
{
	ull v = parseUnsigned(value);
	if (v > 0xff) throw out_of_range("");

	return (Byte)v;
}
bool parseBool(const string& value)
{
	if (value == "true") return true;
	if (value == "false") return false;
	throw std::invalid_argument("");
}

template<class T>
T parse(const string& name, const string& value, ull linenumber, T(*parse_func)(const string&))
{
	try
	{
		return parse_func(value);
	}
	catch (out_of_range&)
	{
		Log::error() << "Error on line " << linenumber << ": value \"" << value << "\" too big for \"" << name << "\".\n";
		throw 0;
	}
	catch (invalid_argument&)
	{
		Log::error() << "Error on line " << linenumber << ": value \"" << value << "\" is invalid for \"" << name << "\".\n";
		throw 0;
	}
	catch (...)
	{
		Log::error() << "Unknown error on line " << linenumber << " evaluating value \"" << value << "\" for \"" << name << "\".\n";
		throw 0;
	}
}

bool Options::Load()
{
	if (loaded) throw mcexception(mcexception::UNSPECIFIED, "server.properties already loaded", "options.cpp", __LINE__);

	bool success = true;
	ifstream opt(optionsFileName);
	if (!opt.is_open())
	{
		Log::warn() << "File \"server.properties\" not found, so one has been generated.\n";
		ofstream out(optionsFileName);
		out << "#lines that begin with a '#' are ignored\n"
			"#remove the '#' at the beginning of a line and modify the value of the property if you want to use a different value for that property instead of the default value\n"
			"#port=25565\n"
			"#max-players=100\n"
			"#main-world-name=world\n"
			"#motd={\"text\":\"A Minecraft server.\"}\n"
			"#ip=0.0.0.0\n"
			"#view-distance=10\n"
			"#chunk-compression=true\n"
			"#network-compression-threshold=128";
		out.close();
		return loaded = true;
	}
	char* line = new char[4096];
	ull linenumber = 0;
	while (opt.getline(line, 4096))
	{
		linenumber++;
		if (*line == commentChar) continue;
		char* str_value = strchr(line, '=');
		if (!str_value)
		{
			Log::error() << "Error on line " << linenumber << ": expected an '='.\n";
			continue;
		}
		*(str_value++) = 0;
		string name = line, value = str_value;
		try
		{
			if (name == "port")
			{
				//_port = parseUShort(name, value, linenumber);
				_port = parse(name, value, linenumber, parseUShort);
			}
			else if (name == "ip")
			{
				_ip = value;
			}
			else if (name == "max-players")
			{
				//_max_players = parseInt(name, value, linenumber);
				_max_players = parse(name, value, linenumber, parseInt);
			}
			else if (name == "motd")
			{
				_motd = value;
			}
			else if (name == "main-world-name")
			{
				_mainWorldName = value;
			}
			else if (name == "view-distance")
			{
				//Byte val = parseByte(name, value, linenumber);
				Byte val = parse(name, value, linenumber, parseByte);
				if (val > Player::maxViewDistance)
				{
					Log::error() << "Error on line " << linenumber << ": value " << value << " is too large for \"" << name << "\". The maximum is \"" << Player::maxViewDistance << "\".\n";
					throw 0;
				}
				_viewDistance = val;
			}
			else if (name == "simulation-distance")
			{
				//_simulationDistance = parseByte(name, value, linenumber);
				_simulationDistance = parse(name, value, linenumber, parseByte);
			}
			else if (name == "chunk-compression")
			{
				_chunkCompression = parse(name, value, linenumber, parseBool);
			}
			else if (name == "network-compression-threshold")
			{
				//_networkCompression = parseShort(name, value, linenumber);
				_networkCompression = parse(name, value, linenumber, parseShort);
			}
			else
			{
				Log::error() << "Error on line " << linenumber << ": unkown property name \"" << name << "\".\n";
				throw 0;
			}
		}
		catch (int e)
		{
			if (e == 0) success = false;
		}
	}
	opt.close();
	if (success) loaded = true;
	delete[] line;
	return loaded;
}
void Options::Unload()
{
	if (!loaded) throw mcexception(mcexception::UNSPECIFIED, "server.properties not loaded", "options.cpp", __LINE__);
}

ush Options::port() { return _port; }
int Options::max_players() { return _max_players; }
const string& Options::mainWorldName() { return _mainWorldName; }
const string& Options::motd() { return _motd; }
const sf::IpAddress& Options::ip() { return _ip; }
Byte Options::viewDistance() { return _viewDistance; }
Byte Options::simulationDistance() { return _simulationDistance; }
bool Options::chunkCompression() { return _chunkCompression; }
short Options::networkCompression() { return _networkCompression; }