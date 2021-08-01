#include "log.h"

using namespace std;

const char log::binFileName[] = "debug.bin", log::txtFileName[] = "debug.txt";
ofstream log::binFile, log::txtFile;

void log::initialize()
{
	binFile.open(binFileName, ios::app | ios::binary);
	txtFile.open(txtFileName, ios::app);
}
void log::txt(const char* data)
{
	txtFile << data << '\n';
}
void log::bin(const char* data, const ull length)
{
	binFile.write(data, length);
}