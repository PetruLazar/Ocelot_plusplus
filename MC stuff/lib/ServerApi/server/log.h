#pragma once
#include "../types/typedefs.h"
#include <fstream>
#include "../types/apidef.h"

class log
{
	SERVER_API static const char binFileName[], txtFileName[];
	SERVER_API static std::ofstream binFile, txtFile;
	SERVER_API static bool initialized;
public:
	SERVER_API static void initialize();
	SERVER_API static bool Initialized();
	SERVER_API static std::ofstream& txt();
	SERVER_API static void bin(const char*, const ull);
};