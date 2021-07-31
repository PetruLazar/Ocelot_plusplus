#include "options.h"
#include <sstream>

const char Options::optionsFileName[] = "server.properties";
std::map<std::string, void*> Options::properties;

void Options::load()
{
	//WIP
	std::ifstream options(optionsFileName);
	std::string line, name, type;
	while (std::getline(options, line))
	{
		std::stringstream stream(line);
		stream >> name >> type;
		std::string value;
		std::getline(stream, value);
		value.erase(0, value.find_first_not_of(' '));
		if (type == "bool")
		{
			bool* p = new bool;
			if (value == "true") *p = true;
			else if (value == "false") *p = false;
			else
			{
				std::cout << "\nUnknown bool value \"" << value << "\"";
				continue;
			}
		}
		else if (type == "sbyte")
		{
			char* p = new char;
		}
		else if (type == "byte")
		{
			byte* p = new byte;
		}
		else if (type == "short")
		{
			short* p = new short;
		}
		else if (type == "ushort")
		{
			ush* p = new ush;
		}
		else if (type == "int")
		{
			int* p = new int;
		}
		else if (type == "uint")
		{
			uint* p = new uint;
		}
		else if (type == "long")
		{
			int64* p = new int64;
		}
		else if (type == "ulong")
		{
			ull* p = new ull;
		}
		else if (type == "float")
		{
			float* p = new float;
		}
		else if (type == "double")
		{
			double* p = new double;
		}
		else if (type == "string")
		{
			std::string* p = new std::string;
		}
		else
		{
			std::cout << "Unkown type \"" << type << "\"";
			continue;
		}
	}
}

const bool& Options::getBool(const std::string&)
{
	throw "WIP";
}
const byte& Options::getByte(const std::string&)
{
	throw "WIP";
}
const char& Options::getSByte(const std::string&)
{
	throw "WIP";
}
const short& Options::getShort(const std::string&)
{
	throw "WIP";
}
const ush& Options::getUShort(const std::string&)
{
	throw "WIP";
}
const int& Options::getInt(const std::string&)
{
	throw "WIP";
}
const uint& Options::getUInt(const std::string&)
{
	throw "WIP";
}
const int64& Options::getLong(const std::string&)
{
	throw "WIP";
}
const ull& Options::getULong(const std::string&)
{
	throw "WIP";
}
const float& Options::getFloat(const std::string&)
{
	throw "WIP";
}
const double& Options::getDouble(const std::string&)
{
	throw "WIP";
}
const std::string& Options::getString(const std::string&)
{
	throw "WIP";
}