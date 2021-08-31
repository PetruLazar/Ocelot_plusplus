#pragma once
#include "types/typedefs.h"
#include <fstream>

class log
{
	static const char binFileName[], txtFileName[];
	static std::ofstream binFile, txtFile;
public:
	static void initialize();
	static std::ofstream& txt();
	static void bin(const char*, const ull);
};