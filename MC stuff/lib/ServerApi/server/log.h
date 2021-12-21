#pragma once

#include <fstream>
#include <chrono>
#include <unordered_map>

#include "../types/typedefs.h"
#include "../types/apidef.h"

#include "SFML/Network/IpAddress.hpp"

class LogAction
{
public:
	enum class ID { flush, newline };

private:
	ID action;

public:
	LogAction(ID action);

	ID getAction();
};

class LogStream
{
private:
	const std::string title;
	std::ofstream* toFile;
	bool enabled = true;

public:
	LogStream(const std::string& title, std::ofstream* toFile);

	void setState(bool state);

	SERVER_API LogStream& operator<<(const bool n);
	SERVER_API LogStream& operator<<(const char n);
	SERVER_API LogStream& operator<<(const unsigned char n);
	SERVER_API LogStream& operator<<(const short n);
	SERVER_API LogStream& operator<<(const unsigned short n);
	SERVER_API LogStream& operator<<(const int n);
	SERVER_API LogStream& operator<<(const unsigned int n);
	SERVER_API LogStream& operator<<(const long n);
	SERVER_API LogStream& operator<<(const unsigned long n);
	SERVER_API LogStream& operator<<(const long long n);
	SERVER_API LogStream& operator<<(const unsigned long long n);
	SERVER_API LogStream& operator<<(const double n);
	SERVER_API LogStream& operator<<(const char* n);
	SERVER_API LogStream& operator<<(const std::string& n);
	SERVER_API LogStream& operator<<(const void* n);
	SERVER_API LogStream& operator<<(const sf::IpAddress& n);

	SERVER_API LogStream& operator<<(LogAction& fs);
};

class Log
{
	SERVER_API static const char binFileName[], logFileName[], debugFileName[];

	SERVER_API static std::ofstream binFile, logFile, debugFile;
	SERVER_API static bool initialized;

	static LogStream noneStream, infoStream, warningStream, errorStream, debugStream;

	static std::string preLogPrint();

	static std::unordered_map<std::string, std::chrono::steady_clock::time_point> benches;

public:
	SERVER_API static void initialize();
	SERVER_API static bool Initialized();

	SERVER_API static LogStream& none();
	SERVER_API static LogStream& info();
	SERVER_API static LogStream& warn();
	SERVER_API static LogStream& debug(bool print = false);
	SERVER_API static LogStream& error();

	SERVER_API static void Bin(const char*, const ull);
	SERVER_API static void Flush();

	SERVER_API static std::string Bench(const std::string& name, bool parentheses = true);

	SERVER_API static LogAction flush;
	SERVER_API static LogAction endl;

	friend LogStream;
};