#include "log.h"

using namespace std;

const char log::binFileName[] = "debug.bin", log::txtFileName[] = "debug.txt";
ofstream log::binFile, log::txtFile;

void log::initialize()
{
	//binFile = malloc(sizeof(ofstream));
	//binFile->basic_ofstream();
	binFile.open(binFileName, /*ios::app |*/ ios::binary);
	txtFile.open(txtFileName /*,ios::app*/);
}
ofstream& log::txt()
{
	return txtFile;
}
void log::bin(const char* data, const ull length)
{
	binFile.write(data, length);
}