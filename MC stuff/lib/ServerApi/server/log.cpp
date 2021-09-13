#include "log.h"
#include "console.h"
#include <iostream>

using namespace std;

const char log::binFileName[] = "debug.bin", log::txtFileName[] = "debug.txt";
ofstream log::binFile, log::txtFile;

DebugStream& DebugStream::operator<<(const bool n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const char n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned char n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const short n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned short n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const int n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned int n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const long long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const unsigned long long n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
DebugStream& DebugStream::operator<<(const char* n)
{
	if (ServerConsole::HasConsole()) cout << n;
	if (log::txtFile.is_open()) log::txtFile << n;
	return *this;
}
static DebugStream debugStream;

void log::initialize()
{
	binFile.open(binFileName, /*ios::app |*/ ios::binary);
	txtFile.open(txtFileName /*,ios::app*/);
}
DebugStream& log::txt()
{
	return debugStream;
}
void log::bin(const char* data, const ull length)
{
	binFile.write(data, length);
}