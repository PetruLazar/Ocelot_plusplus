#pragma once
#include "basic.h"

class Options
{
	static const char optionsFileName[];
	static std::map<std::string, void*> properties;
public:
	static void load();

	static const bool& getBool(const std::string&);
	static const byte& getByte(const std::string&);
	static const char& getSByte(const std::string&);
	static const short& getShort(const std::string&);
	static const ush& getUShort(const std::string&);
	static const int& getInt(const std::string&);
	static const uint& getUInt(const std::string&);
	static const int64& getLong(const std::string&);
	static const ull& getULong(const std::string&);
	static const float& getFloat(const std::string&);
	static const double& getDouble(const std::string&);
	static const std::string& getString(const std::string&);
};