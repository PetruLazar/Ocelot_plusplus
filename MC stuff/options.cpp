#include "options.h"
#include <sstream>

using namespace std;

//const char* elemNotFound = "Setting not found.";
const char Options::optionsFileName[] = "server.properties";
ush Options::_port = 25565;
int Options::_max_players = 100;
std::string Options::_level_name = "world", Options::_motd = "{\"text\":\"A Minecraft server.\"}";
sf::IpAddress Options::_ip = sf::IpAddress::Any;

Options Options::options;

const string Options::version = "\"version\":{\"name\":\"1.17.1\",\"protocol\":756}";

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

Options::Options()
{
	ifstream opt(optionsFileName);
	char* line = new char[4096];
	ull linenumber = 0;
	while (opt.getline(line, 4096))
	{
		linenumber++;
		char* str_value = strchr(line, '=');
		if (!str_value)
		{
			cout << "Error on line " << linenumber << ": expected an '='.\n";
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
				continue;
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
				continue;
			}
		}
		else if (name == "motd")
		{
			_motd = value;
		}
		else
		{
			cout << "Error on line " << linenumber << ": unkown property name \"" << name << "\".\n";
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
const string& Options::level_name() { return _level_name; }
const string& Options::motd() { return _motd; }
const sf::IpAddress& Options::ip() { return _ip; }

/*void Options::load()
{
	//WIP
	ifstream options(optionsFileName);
	string line, name, type;
	while (getline(options, line))
	{
		stringstream stream(line);
		stream >> name >> type;
		string value;
		getline(stream, value);
		value.erase(0, value.find_first_not_of(' '));
		void* ptr = nullptr;
		if (type == "bool")
		{
			if (value == "true") ptr = new bool(true);
			else if (value == "false") ptr = new bool(false);
			else
			{
				cout << "Invalid bool value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "sbyte")
		{
			try
			{
				int v = stoi(value);
				if (v < 0xffffff80i32 || v>0x7f) throw 0;
				ptr = new char(v);
			}
			catch (...)
			{
				cout << "Invalid sbyte value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "byte")
		{
			try
			{
				//long has the same size as int
				uint v = stoul(value);
				if (v > 0xff) throw 0;
				ptr = new byte(v);
			}
			catch (...)
			{
				cout << "Invalid byte value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "short")
		{
			try
			{
				int v = stoi(value);
				if (v > 0x7fff || v < 0xffff8000i32) throw 0;
				ptr = new short(v);
			}
			catch (...)
			{
				cout << "Invalid short value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "ushort")
		{
			try
			{
				uint v = stoul(value);
				if (v > 0xffff) throw 0;
				ptr = new ush(v);
			}
			catch (...)
			{
				cout << "Invalid ushort value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "int")
		{
			try
			{
				int v = stoi(value);
				ptr = new int(v);
			}
			catch (...)
			{
				cout << "Invalid int value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "uint")
		{
			try
			{
				uint v = stoul(value);
				ptr = new uint(v);
			}
			catch (...)
			{
				cout << "Invalid uint value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "long")
		{
			try
			{
				int64 v = stoll(value);
				ptr = new int64(v);
			}
			catch (...)
			{
				cout << "Invalid long value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "ulong")
		{
			try
			{
				ull v = stoull(value);
				ptr = new ull(v);
			}
			catch (...)
			{
				cout << "Invalid ulong value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "float")
		{
			try
			{
				float v = stof(value);
				ptr = new float(v);
			}
			catch (...)
			{
				cout << "Invalid float value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "double")
		{
			try
			{
				double v = stod(value);
				ptr = new double(v);
			}
			catch (...)
			{
				cout << "Invalid double value: \"" << value << "\".\n";
				continue;
			}
		}
		else if (type == "string")
		{
			ptr = new string(value);
		}
		else
		{
			cout << "Unkown type \"" << type << "\".\n";
			continue;
		}

		if (!properties.insert(pair<string, void*>(name, ptr)).second)
		{
			cout << "Could not add \"" << name << "\" as a setting.\n";
		}
	}
	options.close();
}
void Options::free()
{
	for (pair<string, void*> p : properties)
	{
		delete p.second;
	}
	properties.clear();
}

const void* Options::find(const string& key)
{
	Properties::iterator itr = properties.find(key);
	if (itr == properties.end())
	{
		throw elemNotFound;
	}
	return itr->second;
}

const bool& Options::getBool(const string& key)
{
	return *(bool*)find(key);
}
const byte& Options::getByte(const string& key)
{
	return *(byte*)find(key);
}
const char& Options::getSByte(const string& key)
{
	return *(char*)find(key);
}
const short& Options::getShort(const string& key)
{
	return *(short*)find(key);
}
const ush& Options::getUShort(const string& key)
{
	return *(ush*)find(key);
}
const int& Options::getInt(const string& key)
{
	return *(int*)find(key);
}
const uint& Options::getUInt(const string& key)
{
	return *(uint*)find(key);
}
const int64& Options::getLong(const string& key)
{
	return *(int64*)find(key);
}
const ull& Options::getULong(const string& key)
{
	return *(ull*)find(key);
}
const float& Options::getFloat(const string& key)
{
	return *(float*)find(key);
}
const double& Options::getDouble(const string& key)
{
	return *(double*)find(key);
}
const string& Options::getString(const string& key)
{
	return *(string*)find(key);
}*/