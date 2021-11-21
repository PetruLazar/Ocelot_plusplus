
#include <filesystem>
#include <iostream>

#include "recipe.h"
#include "gametags.h"

namespace recipe
{
	std::vector<Recipe*>* Manager::recipes;
	std::vector<mcString>* Manager::recipesIDs;

	void Ingredient::write(char*& buffer) const
	{
		count.write(buffer);
		for (int i = 0; i < count; i++)
			items[i]->write(buffer);
	}

	void Recipe::write(char*& buffer) const
	{
		type.write(buffer);
		recipeID.write(buffer);
	}
	static unsigned recipeID = 0;
	void Manager::loadRecipes()
	{
		recipes = new std::vector<Recipe*>();
		recipesIDs = new std::vector<mcString>();

		std::filesystem::directory_iterator recipesDir("data/recipes/others");

		for (auto const& recipeFile : recipesDir)
		{
			if (!recipeFile.is_regular_file())
				continue;

			std::fstream file(recipeFile.path(), std::ios::in);
			json* data = json::parse(file);

			recipe::Recipe* theRecipe;
			std::string type = (*data)["type"].value();

			if (type == "minecraft:crafting_shapeless") {
				std::cout <<"\n"<< recipeFile.path() << " ";

				mcString group = recipeFile.path().stem().string();
				if (data->has("group"))
				{
					std::cout << "G ";
					group = (*data)["group"].value();
				}

				varInt ingredientCount = (*data)["ingredients"].getSize();
				Ingredient** ingredients = new Ingredient * [ingredientCount];
				for (int i = 0; i < ingredientCount; i++) {
					if ((*data)["ingredients"][i].getType() == json::type::array) {
						json* theArray = &(*data)["ingredients"][i];

						Slot** slotArray = new Slot * [theArray->getSize()];
						for(int j = 0; j < theArray->getSize(); j++)
							slotArray[j] = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["ingredients"][i][j]["item"].value()), 1);
						ingredients[i] = new Ingredient(theArray->getSize(), slotArray);
					} 
					else { //compound
						Slot** slotArray = new Slot * [1];
						int slotArraySize = 1;

						if ((*data)["ingredients"][i].has("item"))
							slotArray[0] = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["ingredients"][i]["item"].value()), 1);
						else { //tag
							delete[] slotArray;

							std::string tagName = (*data)["ingredients"][i]["tag"].value();
							std::cout << "\n" << tagName;

							const TagGroup::Tag* theTag = nullptr; //search it

							if (theTag == nullptr)
								throw "tag not found, recipe couldn't be parsed";


							/*for (const TagGroup& t : TagGroup::defaultTags) {
								for (const TagGroup::Tag& at : t.tags) {
									if(at.name == tagName)
										std::cout << "\n" << at.name;
								}
							}*/
						}

						ingredients[i] = new Ingredient(slotArraySize, slotArray);
					}

					

					/*if ((*data)["ingredients"][i].exists("item"))
						std::cout << "i: "<<(*data)["ingredients"][i]["item"].value() << "\n";
					else if ((*data)["ingredients"][i].exists("tag"))
						std::cout << "t: " << (*data)["ingredients"][i]["tag"].value() << "\n";*/
				}

				varInt resultCount = 1;
				if ((*data)["result"].has("count"))
					resultCount = (*data)["result"]["count"].iValue();
				Slot* result = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["result"]["item"].value()), resultCount);

				theRecipe = new recipe::crafting_shapeless(recipeFile.path().stem().string(), group, ingredientCount, ingredients, result);
				recipesIDs->emplace_back(recipeFile.path().stem().string());
			} else if (type == "minecraft:crafting_shaped") {

			} else if (type == "minecraft:smelting" || type == "minecraft:blasting" || type == "minecraft:smoking" || type == "minecraft:campfire_cooking") {

			} else if (type == "minecraft:stonecutting") {

			} else if (type == "minecraft:smithing") {

			}

			recipes->emplace_back(theRecipe);
		}
	}

	void Manager::unloadRecipes()
	{
		for (Recipe* r : *recipes)
			delete r;

		delete recipes;
	}

	void crafting_shapeless::write(char*& buffer) const
	{
		Recipe::write(buffer);
		group.write(buffer);
		ingredientCount.write(buffer);
		for (int i = 0; i < ingredientCount; i++)
			ingredients[i]->write(buffer);
		result->write(buffer);
	}

	void crafting_shaped::write(char*& buffer) const
	{
		Recipe::write(buffer);
		width.write(buffer);
		height.write(buffer);
		group.write(buffer);
		for (int i = 0; i < width * height; i++)
			ingredients[i]->write(buffer);
		result->write(buffer);
	}

	void smelting::write(char*& buffer) const
	{
		Recipe::write(buffer);
		group.write(buffer);
		ingredient->write(buffer);
		result->write(buffer);
		experience.write(buffer);
		cookingTime.write(buffer);
	}

	void blasting::write(char*& buffer) const
	{
		Recipe::write(buffer);
		group.write(buffer);
		ingredient->write(buffer);
		result->write(buffer);
		experience.write(buffer);
		cookingTime.write(buffer);
	}

	void smoking::write(char*& buffer) const
	{
		Recipe::write(buffer);
		group.write(buffer);
		ingredient->write(buffer);
		result->write(buffer);
		experience.write(buffer);
		cookingTime.write(buffer);
	}

	void campfire_cooking::write(char*& buffer) const
	{
		Recipe::write(buffer);
		group.write(buffer);
		ingredient->write(buffer);
		result->write(buffer);
		experience.write(buffer);
		cookingTime.write(buffer);
	}

	void stonecutting::write(char*& buffer) const
	{
		Recipe::write(buffer);
		group.write(buffer);
		ingredient->write(buffer);
		result->write(buffer);
	}

	void smithing::write(char*& buffer) const
	{
		Recipe::write(buffer);
		base->write(buffer);
		addition->write(buffer);
		result->write(buffer);
	}
}
