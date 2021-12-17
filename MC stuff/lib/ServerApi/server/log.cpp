
#include "log.h"

#include "console.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;

const char Log::binFileName[] = "debug.bin", Log::txtFileName[] = "debug.txt";
bool Log::initialized = false;
ofstream Log::binFile, Log::txtFile;

std::unordered_map<std::string, std::chrono::steady_clock::time_point> Log::benches;

LogStream Log::infoStream("info");
LogStream Log::warningStream("warning");
LogStream Log::errorStream("error");
LogStream Log::noneStream("none");

LogAction Log::flush(LogAction::ID::flush);
LogAction Log::endl(LogAction::ID::newline);

LogAction::LogAction(LogAction::ID action) : action(action) {}
LogAction::ID LogAction::getAction() {
	return action;
}

LogStream::LogStream(const std::string& title) : title(title) {}

LogStream& LogStream::operator<<(const bool n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const char n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const unsigned char n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const short n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const unsigned short n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const int n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const unsigned int n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const unsigned long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const long long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const unsigned long long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const double n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const char* n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const std::string& n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const void* n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const sf::IpAddress& n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}

LogStream& LogStream::operator<<(LogAction& n)
{
	switch (n.getAction()) {
	case LogAction::ID::flush:
		if (ServerConsole::HasConsole()) cout << '\n';
		if (Log::txtFile.is_open()) Log::txtFile << '\n';

		Log::txtFile.flush();
		Log::binFile.flush();
		break;
	case LogAction::ID::newline:
		if (ServerConsole::HasConsole()) cout << '\n';
		if (Log::txtFile.is_open()) Log::txtFile << '\n';
		break;
	}
	
	return *this;
}

void Log::initialize()
{
	binFile.open(binFileName, /*ios::app |*/ ios::binary);
	txtFile.open(txtFileName /*,ios::app*/);

	initialized = true;
}
bool Log::Initialized()
{
	return initialized;
}
void Log::preLogPrint()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	tm tt;
	localtime_s(&tt, &in_time_t);

	std::cout << std::put_time(&tt, "%EX");
}

LogStream& Log::none()
{
	return infoStream;
}
LogStream& Log::info()
{
	preLogPrint();
	cout << "  [INFO]: ";
	return infoStream;
}
LogStream& Log::warn()
{
	preLogPrint();
	cout << "  [WARN]: ";
	return infoStream;
}
LogStream& Log::error()
{
	preLogPrint();
	cout << " [ERROR]: ";
	return infoStream;
}

void Log::Bin(const char* data, const ull length)
{
	binFile.write(data, length);
}
void Log::Flush()
{
	binFile.flush();
	txtFile.flush();
}
std::string Log::Bench(const std::string& name, bool parentheses)
{
	if (benches.find(name) == benches.end()) {
		benches[name] = std::chrono::high_resolution_clock::now();
		return "";
	}

	double msTime = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - benches[name]).count();

	benches.erase(name);

	if (parentheses)
		return "(Took " + std::to_string(msTime) + " ms)";
	
	return "Took " + std::to_string(msTime) + "ms.";
}