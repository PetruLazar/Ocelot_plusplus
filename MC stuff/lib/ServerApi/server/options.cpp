
#include <iostream>
#include <fstream>

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

Options Options::options;

const string Options::version = "\"version\":{\"name\":\"1.18.2\",\"protocol\":758}";
bool Options::allowJoin = true;

ull parseUnsigned(const string& name, const string& value, ull linenumber)
{
	ull v;
	try
	{
		v = stoull(value);
	}
	catch (out_of_range)
	{
		cout << "Error on line " << linenumber << ": value " << value << " too big for \"" << name << "\".\n";
		throw 0;
	}
	catch (invalid_argument)
	{
		cout << "Error on line " << linenumber << ": value " << value << " is invalid for \"" << name << "\".\n";
		throw 0;
	}
	catch (...)
	{
		cout << "Unknown error on line " << linenumber << " during value evaluation.\n";
		throw 0;
	}
	return v;
}
int64 parse(const string& name, const string& value, ull linenumber)
{
	int64 v;
	try
	{
		v = stoll(value);
	}
	catch (out_of_range)
	{
		cout << "Error on line " << linenumber << ": value " << value << " too big for \"" << name << "\".\n";
		throw 0;
	}
	catch (invalid_argument)
	{
		cout << "Error on line " << linenumber << ": value " << value << " is invalid for \"" << name << "\".\n";
		throw 0;
	}
	catch (...)
	{
		cout << "Unknown error on line " << linenumber << " during value evaluation.\n";
		throw 0;
	}
	return v;
}
ush parseUShort(const string& name, const string& value, ull linenumber)
{
	ull v;
	try
	{
		v = parseUnsigned(name, value, linenumber);
		if (v > 0xffff) throw out_of_range(0);
	}
	catch (out_of_range)
	{
		cout << "Error on line " << linenumber << ": value " << v << " too big for \"" << name << "\".\n";
		throw 0;
	}
	catch (...)
	{
		throw 0;
	}
	return (ush)v;
}
int parseInt(const string& name, const string& value, ull linenumber)
{
	int64 v;
	try
	{
		v = parse(name, value, linenumber);
		if (v > 0x7fffffffi64 || v < 0xffffffff80000000i64) throw out_of_range(0);
	}
	catch (out_of_range)
	{
		cout << "Error on line " << linenumber << ": value " << v << " too big for \"" << name << "\".\n";
		throw 0;
	}
	catch (...)
	{
		throw 0;
	}
	return (int)v;
}
int parseShort(const string& name, const string& value, ull linenumber)
{
	int64 v;
	try
	{
		v = parse(name, value, linenumber);
		if (v > 0x7fffi64 || v < 0xffffffffffff8000i64) throw out_of_range(0);
	}
	catch (out_of_range)
	{
		cout << "Error on line " << linenumber << ": value " << v << " too big for \"" << name << "\".\n";
		throw 0;
	}
	catch (...)
	{
		throw 0;
	}
	return (int)v;
}
Byte parseByte(const string& name, const string& value, ull linenumber)
{
	ull v;
	try
	{
		v = parseUnsigned(name, value, linenumber);
		if (v > 0xff) throw out_of_range(0);
	}
	catch (out_of_range)
	{
		cout << "Error on line " << linenumber << ": value " << v << " too big for \"" << name << "\".\n";
		throw 0;
	}
	catch (...)
	{
		throw 0;
	}
	return (Byte)v;
}

Options::Options()
{
	ifstream opt(optionsFileName);
	if (!opt.is_open())
	{
		Log::warn() << "File \"server.properties\" not found, so one has been generated.\n";
		ofstream out(optionsFileName);
		out << "#lines that begin with a '#' are ignored\n#remove the '#' at the beginning of a line and modify the value of the property if you want to use a different value for that property instead of the default value\n#port=25565\n#max-players=100\n#main-world-name=world\n#motd={\"text\":\"A Minecraft server.\"}\n#ip=0.0.0.0\n#view-distance=10\n#chunk-compression=true\n#network-compression-threshold=128";
		out.close();
		return;
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
		if (name == "port")
		{
			try
			{
				_port = parseUShort(name, value, linenumber);
			}
			catch (...)
			{
			}
		}
		else if (name == "ip")
		{
			_ip = value;
		}
		else if (name == "max-players")
		{
			try
			{
				_max_players = parseInt(name, value, linenumber);
			}
			catch (...)
			{
			}
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
			try
			{
				_viewDistance = parseByte(name, value, linenumber);
			}
			catch (...)
			{
			}
		}
		else if (name == "view-distance")
		{
			try
			{
				_simulationDistance = parseByte(name, value, linenumber);
			}
			catch (...)
			{
			}
		}
		else if (name == "chunk-compression")
		{
			if (value == "true")
			{
				_chunkCompression = true;
			}
			else if (value == "false")
			{
				_chunkCompression = false;
			}
			else
			{
				cout << "Error on line " << linenumber << ": value " << value << " is invalid for \"" << name << "\".\n";
			}
		}
		else if (name == "network-compression-threshold")
		{
			try
			{
				_networkCompression = parseShort(name, value, linenumber);
			}
			catch (...)
			{
			}
		}
		else
		{
			Log::error() << "Error on line " << linenumber << ": unkown property name \"" << name << "\".\n";
		}
	}
	opt.close();
	delete[] line;
}
Options::~Options()
{

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