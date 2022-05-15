
#include "log.h"

#include "../server/server.h"
#include "../server/options.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <thread>
#include <mutex>
#include <sstream>

using namespace std;

std::mutex printMutex;

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
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

	return *this;
}
LogStream& LogStream::operator<<(const sf::IpAddress& n)
{
	if (this->enabled) {
		if (Server::HasConsole()) cout << n;
		if (toFile->is_open()) *toFile << n;
	}

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

	Log::info() <<		  "  ____          __     __    __    __ \n"
					"\t\t\t / __ \\_______ / /__  / /___/ /___/ /_\n"
					"\t\t\t/ /_/ / __/ -_) / _ \\/ __/_  __/_  __/\n"
					"\t\t\t\\____/\\__/\\__/_/\\___/\\__/ /_/   /_/   \n" << Log::endl;
	Log::info() << versionName << " (x64) (Windows)" << Log::endl;
	Log::info() << "Minecraft version: " << Options::currentVersion() << " (" << Options::currentProtocol() << ")" << Log::endl;

	initialized = true;
}
bool Log::Initialized()
{
	return initialized;
}

void Log::preLogPrint(LogStream& stream)
{
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	char buffer[10] = "[";

	strftime(buffer + 1, sizeof(buffer), "%H:%M:%S", &newtime);

	std::ostringstream ss;
	ss << std::this_thread::get_id();

	stream << buffer << ' ';
	colorPrint(stream, ss.str(), color::GRAY);
	stream << '\t';
}

void Log::colorPrint(LogStream& stream, const std::string& text, color c) {
	if (Server::HasConsole() && stream.getState()) {
		const std::lock_guard<std::mutex> lock(printMutex);

		switch (c) {
		case color::BLACK:
			std::cout << "\033[30m";
			break;
		case color::RED:
			std::cout << "\033[31m";
			break;
		case color::GREEN:
			std::cout << "\033[32m";
			break;
		case color::BLUE:
			std::cout << "\033[34m";
			break;
		case color::WHITE:
			std::cout << "\033[97m";
			break;
		case color::GRAY:
			std::cout << "\033[90m";
			break;
		case color::YELLOW:
			std::cout << "\033[33m";
			break;
		}

		stream << text;

		std::cout << "\033[0m";
	}
}

LogStream& Log::none()
{
	return infoStream;
}
LogStream& Log::info()
{
	preLogPrint(infoStream);
	colorPrint(infoStream, " INFO", color::WHITE);
	infoStream << "]: ";

	return infoStream;
}
LogStream& Log::warn()
{
	preLogPrint(warningStream);
	colorPrint(warningStream, " WARN", color::YELLOW);
	warningStream << "]: ";

	return warningStream;
}
LogStream& Log::error()
{
	preLogPrint(errorStream);
	colorPrint(errorStream, "ERROR", color::RED);
	errorStream << "]: ";

	return errorStream;
}
LogStream& Log::debug(bool print)
{
	debugStream.setState(print);

	preLogPrint(debugStream);
	colorPrint(debugStream, "DEBUG", color::BLUE);
	debugStream << "]: ";

	return debugStream;
}

void LogStream::setState(bool state) {
	this->enabled = state;
}
bool LogStream::getState() {
	return enabled;
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

	auto msTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - benches[name]).count();
	benches.erase(name);

	if (parentheses)
		return "(Took " + std::to_string(msTime) + " ms)";
	
	return "Took " + std::to_string(msTime) + "ms.";
}
