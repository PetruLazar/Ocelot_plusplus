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