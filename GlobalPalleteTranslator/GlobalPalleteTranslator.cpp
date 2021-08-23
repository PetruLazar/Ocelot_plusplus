#include <iostream>
#include "../MC stuff/json.h"

using namespace std;

int main()
{
	//read the global palette in json format from the minecraft generated report
	fstream paletteFile("../reports/blocks.json", ios::in);
	json& palette = *json::parse(paletteFile);
	paletteFile.close();

	string enumContent,
		to_stringContent,
		from_stringContent;

	//take every block in the palette
	int size = (int)palette.getSize();

	for (int i = 0; i < size; i++)
	{
		//obtaining block name
		json& block = palette[i];
		std::string blockName = block.getName();
		for (char& c : blockName)
			if (c == ':')
				c = '_';

		//check if the block has properties or not
		try
		{
			json& props = block["properties"];
			//block has properties

		}
		catch (...)
		{
			//block does not have properties

		}
	}

	return 0;
}