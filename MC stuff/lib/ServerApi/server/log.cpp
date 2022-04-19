
#include "log.h"

#include "server.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;

bool		Log::initialized = false;
const char	Log::binFileName[] = "log.bin", Log::logFileName[] = "log.txt", Log::debugFileName[] = "debug.txt";
ofstream	Log::binFile, Log::logFile, Log::debugFile;

std::unordered_map<std::string, std::chrono::steady_clock::time_point> Log::benches;

LogStream	Log::infoStream("info", &Log::logFile);
LogStream	Log::warningStream("warning", &Log::logFile);
LogStream	Log::errorStream("error", &Log::logFile);
LogStream	Log::noneStream("none", &Log::logFile);
LogStream	Log::debugStream("debug", &Log::debugFile);

LogAction	Log::flush(LogAction::ID::flush);
LogAction	Log::endl(LogAction::ID::newline);

LogAction::LogAction(LogAction::ID action) : action(action) {}
LogAction::ID LogAction::getAction() {
	return action;
}

LogStream::LogStream(const std::string& title, std::ofstream* toFile = &Log::logFile) : title(title), toFile(toFile) {}

LogStream& LogStream::operator<<(const bool n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const char n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const unsigned char n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const short n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const unsigned short n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const int n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const unsigned int n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const long n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const unsigned long n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const long long n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const unsigned long long n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const double n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const char* n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const std::string& n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}
	
	return *this;
}
LogStream& LogStream::operator<<(const std::thread::id& n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const void* n)
{
	if (Server::HasConsole()) cout << n;
	if (toFile->is_open()) *toFile << n;
	return *this;
}
LogStream& LogStream::operator<<(const sf::IpAddress& n)
{
	if (Server::HasConsole()) cout << n;
	if (toFile->is_open()) *toFile << n;
	return *this;
}

LogStream& LogStream::operator<<(LogAction& n)
{
	if (this->enabled) {
		switch (n.getAction()) {
		case LogAction::ID::flush:
			if (Server::HasConsole()) cout << '\n';
			if (toFile->is_open()) *toFile << '\n';

			Log::Flush();
			break;
		case LogAction::ID::newline:
			if (Server::HasConsole()) cout << '\n';
			if (toFile->is_open()) *toFile << '\n';
			break;
		}
	}
	
	return *this;
}

void Log::initialize()
{
	binFile.open(binFileName, /*ios::app |*/ ios::binary);
	logFile.open(logFileName /*,ios::app*/);
	debugFile.open(debugFileName /*,ios::app*/);

	initialized = true;
}
bool Log::Initialized()
{
	return initialized;
}
std::string Log::preLogPrint()
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	char buffer[10] = "[";

	strftime(buffer + 1, sizeof(buffer), "%H:%M:%S", &newtime);

	return buffer;
}

LogStream& Log::none()
{
	return infoStream;
}
LogStream& Log::info()
{
	infoStream << preLogPrint() << "  INFO]: ";
	return infoStream;
}
LogStream& Log::warn()
{
	warningStream << preLogPrint() << "  WARN]: ";
	return warningStream;
}
LogStream& Log::error()
{
	errorStream << preLogPrint() << " ERROR]: ";
	return errorStream;
}
LogStream& Log::debug(bool print)
{
	debugStream.setState(print);

	debugStream << preLogPrint() << " DEBUG]: ";
	return debugStream;
}

void LogStream::setState(bool state) {
	this->enabled = state;
}

void Log::Bin(const char* data, const ull length)
{
	binFile.write(data, length);
}
void Log::Flush()
{
	binFile.flush();
	logFile.flush();
	debugFile.flush();
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
