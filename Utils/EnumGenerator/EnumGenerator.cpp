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
	ofstream out(argc == 3 ? args[2] : "registry_enums.txt");
	if (!out.is_open())
	{
		cout << "Cannot open the output file.";
		return 3;
	}

	//search for properties
	vector<json*> list;
	//take each block in the json
	ull size = registries.getSize();
	for (ull i = 0; i < size; i++)
	{
		try
		{
			json& properties = registries[i]["properties"];
			ull size = properties.getSize();
			for (ull j = 0; j < size; j++)
			{
				json& prop = properties[j];
				bool already = false;
				for (json* entry : list)
				{
					if (entry->getName() == prop.getName() && entry->getSize() == prop.getSize())
					{
						already = true;
						break;
					}
				}
				if (already) continue;
				list.emplace_back(&prop);
			}
		}
		catch (const char[]) {}
	}

	for (json* prop : list) out << prop->to_string() << '\n';

	delete &registries;
	registriesFile.close();
	out.close();
	return 0;

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
		for (ull i = 0u; i < entryCount; i++)
		{
			json& entry = registry[(int)i];
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
	out.close();
	return 0;
}