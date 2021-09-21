#pragma once
#include "../types/typedefs.h"
#include <fstream>
#include "../types/apidef.h"

class DebugStream
{
public:
	SERVER_API DebugStream& operator<<(const bool n);
	SERVER_API DebugStream& operator<<(const char n);
	SERVER_API DebugStream& operator<<(const unsigned char n);
	SERVER_API DebugStream& operator<<(const short n);
	SERVER_API DebugStream& operator<<(const unsigned short n);
	SERVER_API DebugStream& operator<<(const int n);
	SERVER_API DebugStream& operator<<(const unsigned int n);
	SERVER_API DebugStream& operator<<(const long n);
	SERVER_API DebugStream& operator<<(const unsigned long n);
	SERVER_API DebugStream& operator<<(const long long n);
	SERVER_API DebugStream& operator<<(const unsigned long long n);
	SERVER_API DebugStream& operator<<(const char* n);
	SERVER_API DebugStream& operator<<(const void* n);
	SERVER_API DebugStream& operator<<(const std::string& n);
};

class Log
{
	SERVER_API static const char binFileName[], txtFileName[];
	SERVER_API static std::ofstream binFile, txtFile;
	SERVER_API static bool initialized;
public:
	SERVER_API static void initialize();
	SERVER_API static bool Initialized();
	SERVER_API static DebugStream& txt();
	SERVER_API static void bin(const char*, const ull);

	friend DebugStream;
};