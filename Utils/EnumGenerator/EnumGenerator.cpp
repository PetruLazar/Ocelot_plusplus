#include <iostream>
#include <fstream>

#include "json.h"

using namespace std;

const char* registryMapping[][2] = {
	//registry name, enum name
	{ "minecraft:block", "Block" },
	{ "minecraft:item", "Item" },
};
const ull registryMappingSize = sizeof(registryMapping) / sizeof(registryMapping[0]);

int main(int argc, char** args)
{
	//check for argument
	if (argc < 2 || argc > 3)
	{
		cout << "Give the path to the registries file as an argument.";
		return 1;
	}

	//open the file
	fstream registriesFile(args[1], ios::in);
	if (!registriesFile.is_open())
	{
		cout << "Cannot open registries file.";
		return 2;
	}

	//read and parse the file
	json& registries = *json::parse(registriesFile);

	//open the output file
	ofstream out(argc == 3 ? args[2] : "registry_enums");
	if (!out.is_open())
	{
		cout << "Cannot open the output file.";
		return 3;
	}

	//produce output
	//start the Item enum
	out << "#pragma once";

	//generate the enums
	for (ull r = 0; r < registryMappingSize; r++)
	{
		//start the enum
		out << "\n\nenum class " << registryMapping[r][1] << "\n{\n";

		//fill the enum
		json& registry = registries[registryMapping[r][0]]["entries"];
		ull entryCount = registry.getSize();
		for (ull i = 0; i < entryCount; i++)
		{
			json& entry = registry[i];
			string name = entry.getName();
			//replace the ':' in the entry's name with a '_'
			name[9] = '_';
			out << '\t' << name << " = " << entry["protocol_id"].iValue() << ",\n";
		}

		//end the enum
		out << "};";
	}

	//clean up
	delete &registries;
	registriesFile.close();
	return 0;
}