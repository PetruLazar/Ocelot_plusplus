#pragma once
#include <filesystem>
#include <fstream>
#include "types/varData.h"

using namespace std;
using namespace std::filesystem;

void convertWorldHeader(const path& filename)
{
	//write a varInt(0) at the beginning of the file
	//string filename = "worlds\\" + name + "\\characteristics.bin";
	fstream worldMain(filename, ios::binary | ios::in | ios::ate);
	if (!worldMain.is_open())
		throw 0;
	ull filesize = worldMain.tellg();
	worldMain.seekg(0);
	char* filedata = new char[filesize];
	worldMain.read(filedata, filesize);
	worldMain.close();

	worldMain.open(filename, ios::binary | ios::out);
	if (!worldMain.is_open())
	{
		delete[] filedata;
		throw 0;
	}
	varInt(0).write(worldMain);
	worldMain.write(filedata, filesize);
	delete[] filedata;
	worldMain.close();
}