#include <iostream>
#include "types/utils.h"
#include "world.h"
#include "worldHeader.h"

void convertFromPrevious(const path& wldPath, int targetVersion)
{
	switch (targetVersion)
	{
	case 0:
		throw "Invalid version -1";
	case 1:
		//from v0 to v1
		convertWorld0to1(wldPath);
		break;
	default:
		throw "Unknown target version";
	}
}
void convertWorld(const path& wldPath)
{
	//read current version
	ifstream in(wldPath / "characteristics.bin");
	varInt version;
	version.read(in);
	in.close();

	//convert from world version to the currently used world version, one by one
	for (int i = version + 1; i <= World::currentWorldVersion; i++)
		convertFromPrevious(wldPath, i);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "Build folder not provided";
		return 1;
	}
	//make sure the program was compiled with the target version that is intented to be used by the caller
	cout << "Target version for conversion is " << World::currentWorldVersion << ". Continue? (y/n) ";
	do
	{
		string answer;
		cin >> answer;
		if (answer == "y")
			break;
		if (answer == "n")
			return 2;
	} while (true);
	string buildFolder = argv[1];
	for (auto& config : directory_iterator(buildFolder)) // take each configuration in the build folder
	{
		//backup worlds folder
		path backupPath = config.path() / "worlds_BACKUP";
		if (exists(backupPath))
		{
			uint prefix;
			for (prefix = 1; prefix <= 1000; prefix++)
			{
				path prefixed = backupPath.string() + to_string(prefix);
				if (!exists(prefixed))
				{
					backupPath = prefixed;
					break;
				}
			}
			if (prefix == 1001)
			{
				//not found
				cout << "Error: Could not backup world folder in configuration " << config.path().filename().string();
				return 3;
			}
			//found
		}
		cout << "Backing up worlds for configuration " << config.path().filename().string() << " under folder " << backupPath.string() << '\n';
		error_code err;
		copy(config.path() / "worlds", backupPath, copy_options::recursive, err);
		if (err)
		{
			cout << "Error: Backup failed for worlds in configuration " << config.path().filename().string();
			return 3;
		}

		for (auto& world : directory_iterator(config.path() / "worlds")) //take each world in the worlds folder
		{
			try
			{
				if (world.is_directory())
					convertWorld(world.path());
			}
			catch (int e)
			{
				if (e != 0) throw;
				cout << "Problems opening the characteristics file for" << world.path().stem() << '\n';
			}
			catch (const char* msg)
			{
				cout << "Error: " << msg;
				return 3;
			}
		}
	}
	return 0;
}