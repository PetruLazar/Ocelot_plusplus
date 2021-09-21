#include "Log.h"
#include "console.h"
#include <iostream>

using namespace std;

const char Log::binFileName[] = "debug.bin", Log::txtFileName[] = "debug.txt";
ofstream Log::binFile, Log::txtFile;

DebugStream& DebugStream::operator<<(const bool n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const char n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned char n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const short n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned short n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const int n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned int n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const long long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned long long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const char* n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const void* n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const std::string& n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (Log::txtFile.is_open()) Log::txtFile << n;
	return *this;
}
static DebugStream debugStream;

void Log::initialize()
{
	binFile.open(binFileName, /*ios::app |*/ ios::binary);
	txtFile.open(txtFileName /*,ios::app*/);
}
DebugStream& Log::txt()
{
	return debugStream;
}
void Log::bin(const char* data, const ull length)
{
	binFile.write(data, length);
}

/*

700 is entering world world
Player F90 entering sight of 700
Sight of 700 is now 1
Player 700 entering sight of F90
Sight of F90 is now 1
Player list for world is now 2
Player 700 is leaving world world
Player 700 exiting sight of F90
Sight of F90 is now 0
Player F90 exiting sight of 700
Sight of 700 is now 0
Player list of world is now 1
700 is entering world old
Player list for old is now 1
Player 700 is leaving world old
Player list of old is now 0
700 is entering world world
Player 700 entering sight of 700
Sight of 700 is now 1
Player 700 entering sight of 700
Sight of 700 is now 2
Player list for world is now 2
Player 700 is leaving world world
Player 700 exiting sight of 700
Sight of 700 is now 1
Player 700 exiting sight of 700
Sight of 700 is now 0
Player 700 exiting sight of 700
crash

*/