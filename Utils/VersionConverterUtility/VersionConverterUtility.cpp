#include "worldHeader.h"
#include <iostream>
#include "types/utils.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "Build folder not provided";
		return 1;
	}
	string buildFolder = argv[1];
	for (auto& config : directory_iterator(buildFolder)) // take each configuration in the build folder
	{
		for (auto& world : directory_iterator(config.path() / "worlds")) //take each world in the worlds folder
		{
			try
			{
				if (world.is_directory()) convertWorldHeader(world.path() / "characteristics.bin");
			}
			catch (int e)
			{
				if (e != 0) throw;
				cout << "Problems opening the characteristics file for" << world.path().stem() << '\n';
			}
		}
	}
	return 0;
}