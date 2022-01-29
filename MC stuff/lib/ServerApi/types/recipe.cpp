
#include <filesystem>
#include <iostream>

#include "recipe.h"
#include "gametags.h"

namespace recipe
{
	const std::string Recipe::crafting_shapeless = "minecraft:crafting_shapeless", Recipe::crafting_shaped = "minecraft:crafting_shaped", Recipe::smelting = "minecraft:smelting", Recipe::blasting = "minecraft:blasting", Recipe::smoking = "minecraft:smoking", Recipe::campfire_cooking = "minecraft:campfire_cooking", Recipe::stonecutting = "minecraft:stonecutting", Recipe::smithing = "minecraft:smithing",
		Recipe::crafting_special_armordye = "minecraft:crafting_special_armordye", Recipe::crafting_special_bookcloning = "minecraft:crafting_special_bookcloning", Recipe::crafting_special_mapcloning = "minecraft:crafting_special_mapcloning", Recipe::crafting_special_mapextending = "minecraft:crafting_special_mapextending", Recipe::crafting_special_firework_rocket = "minecraft:crafting_special_firework_rocket",
		Recipe::crafting_special_firework_star = "minecraft:crafting_special_firework_star", Recipe::crafting_special_firework_star_fade = "minecraft:crafting_special_firework_star_fade", Recipe::crafting_special_repairitem = "minecraft:crafting_special_repairitem", Recipe::crafting_special_tippedarrow = "minecraft:crafting_special_tippedarrow", Recipe::crafting_special_bannerduplicate = "minecraft:crafting_special_bannerduplicate",
		Recipe::crafting_special_banneraddpattern = "minecraft:crafting_special_banneraddpattern", Recipe::crafting_special_shielddecoration = "minecraft:crafting_special_shielddecoration", Recipe::crafting_special_shulkerboxcoloring = "minecraft:crafting_special_shulkerboxcoloring", Recipe::crafting_special_suspiciousstew = "minecraft:crafting_special_suspiciousstew";

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

	Ingredient* Manager::getItemOrTag(json* data)
	{
		Slot** slotArray = new Slot * [1];
		ull slotArraySize = 1;

		if (data->getType() == json::type::array) {
			delete[] slotArray;
			slotArraySize = data->getSize();
			slotArray = new Slot * [slotArraySize];

			for (int j = 0; j < slotArraySize; j++) //array obj can be only item?
				slotArray[j] = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)[j]["item"].value()), 1);
			return new Ingredient(slotArraySize, slotArray);
		}
		//else, compound

		if (data->has("item"))
			slotArray[0] = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["item"].value()), 1);
		else { //tag
			std::string tagName = (*data)["tag"].value();

			const TagGroup::Tag* theTag = TagGroup::getTag(TagGroup::itemTags, tagName); //search it

			if (theTag == nullptr)
				throw "tag not found, recipe couldn't be parsed";

			delete[] slotArray;
			slotArraySize = theTag->entries.size();
			slotArray = new Slot * [slotArraySize];
			for (int j = 0; j < slotArraySize; j++)
				slotArray[j] = new Slot(true, theTag->entries[j], 1);
		}

		return new Ingredient(slotArraySize, slotArray);
	}

	void Manager::addRecipe(const mcString& id, recipe::Recipe* recipe)
	{
		recipesIDs->emplace_back(id);
		recipes->emplace_back(recipe);
	}

	void Manager::loadStaticRecipes()
	{
		//std::cout << "acacia planks: " << Registry::getId(Registry::itemRegistry, "minecraft:acacia_planks") << std::endl; //26
		//std::cout << "acacia boat: " << Registry::getId(Registry::itemRegistry, "minecraft:acacia_boat") << std::endl; //674
		//Manager::addRecipe("acacia_boat", new recipe::crafting_shaped("acacia_boat", 3, 2, "boat", new Ingredient * [3 * 2]{ new Slot(true, 674, 1) }, new Slot(true, 674, 1)));
	}

	void Manager::loadDynamicRecipes()
	{
		std::filesystem::directory_iterator recipesDir("data/recipes");

		for (auto const& recipeFile : recipesDir)
		{
			if (!recipeFile.is_regular_file())
				continue;

			std::fstream file(recipeFile.path(), std::ios::in);
			json* data = json::parse(file);

			recipe::Recipe* theRecipe;
			std::string type = (*data)["type"].value();

			if (type == Recipe::crafting_shapeless) {
				mcString group = recipeFile.path().stem().string();
				if (data->has("group"))
					group = (*data)["group"].value();
				varInt resultCount = 1;
				if ((*data)["result"].has("count"))
					resultCount = (*data)["result"]["count"].iValue();
				Slot* result = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["result"]["item"].value()), resultCount);

				varInt ingredientCount = (*data)["ingredients"].getSize();
				Ingredient** ingredients = new Ingredient * [ingredientCount];
				for (int i = 0; i < ingredientCount; i++)
					ingredients[i] = getItemOrTag(&(*data)["ingredients"][i]);

				theRecipe = new recipe::crafting_shapeless(recipeFile.path().stem().string(), group, ingredientCount, ingredients, result);
			}
			else if (type == Recipe::crafting_shaped) {
				mcString group = recipeFile.path().stem().string();
				if (data->has("group"))
					group = (*data)["group"].value();
				varInt resultCount = 1;
				if ((*data)["result"].has("count"))
					resultCount = (*data)["result"]["count"].iValue();
				Slot* result = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["result"]["item"].value()), resultCount);

				int height = (*data)["pattern"].getSize();
				int width = 1;
				for (int i = 0; i < height; i++)
				{
					int rowSize = (*data)["pattern"][i].value().size();
					if ((*data)["pattern"][i].value().find('\\') != std::string::npos)
						rowSize = rowSize - 5;

					if (rowSize > width)
						width = rowSize;
				}

				Ingredient** ingredients = new Ingredient * [width * height];
				for (int x = 0; x < height; x++) {
					std::string row = (*data)["pattern"][x].value();
					int accessY = 0;
					for (int y = 0; y < width; y++) {
						if (row[accessY] == ' ') {
							Slot** slotArray = new Slot * [1];
							slotArray[0] = new Slot(false, 0);
							ingredients[y + x * width] = new Ingredient(1, slotArray);
						}
						else if (row[accessY] == '\\') {
							ingredients[y + x * width] = getItemOrTag(&(*data)["key"][row.substr(accessY, 6)]);
							accessY = accessY + 5;
						}
						else
							ingredients[y + x * width] = getItemOrTag(&(*data)["key"][std::string(1, row[accessY])]);

						accessY += 1;
					}
				}

				theRecipe = new recipe::crafting_shaped(recipeFile.path().stem().string(), width, height, group, ingredients, result);
			}
			else if (type == Recipe::smelting || type == Recipe::blasting || type == Recipe::smoking || type == Recipe::campfire_cooking) {
				mcString group = recipeFile.path().stem().string();
				if (data->has("group"))
					group = (*data)["group"].value();
				Slot* result = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["result"].value()), 1); //maximum of 1 item?
				bfloat experience = (*data)["experience"].dValue();
				varInt cookingTime = (*data)["cookingtime"].iValue();

				Ingredient* ingredient = getItemOrTag(&(*data)["ingredient"]);

				if (type == Recipe::smelting)
					theRecipe = new recipe::smelting(recipeFile.path().stem().string(), group, ingredient, result, experience, cookingTime);
				else if (type == Recipe::blasting)
					theRecipe = new recipe::blasting(recipeFile.path().stem().string(), group, ingredient, result, experience, cookingTime);
				else if (type == Recipe::smoking)
					theRecipe = new recipe::smoking(recipeFile.path().stem().string(), group, ingredient, result, experience, cookingTime);
				else //campfire_cooking
					theRecipe = new recipe::campfire_cooking(recipeFile.path().stem().string(), group, ingredient, result, experience, cookingTime);
			}
			else if (type == Recipe::stonecutting) {
				mcString group = recipeFile.path().stem().string();
				if (data->has("group"))
					group = (*data)["group"].value();
				varInt resultCount = 1;
				if ((*data).has("count"))
					resultCount = (*data)["count"].iValue();
				Slot* result = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["result"].value()), resultCount);

				Ingredient* ingredient = getItemOrTag(&(*data)["ingredient"]);

				theRecipe = new recipe::stonecutting(recipeFile.path().stem().string(), group, ingredient, result);
			}
			else if (type == Recipe::smithing) {
				Ingredient* base = getItemOrTag(&(*data)["base"]);
				Ingredient* addition = getItemOrTag(&(*data)["addition"]);

				Slot* result = new Slot(true, Registry::getId(Registry::itemRegistry, (*data)["result"]["item"].value()), 1);

				theRecipe = new recipe::smithing(recipeFile.path().stem().string(), base, addition, result);
			}
			else //other recipies that dont have any extra data
				theRecipe = new recipe::Recipe(type, recipeFile.path().stem().string());

			Manager::addRecipe(recipeFile.path().stem().string(), theRecipe);
		}
	}

	const Recipe* Manager::getRecipe(const std::string& recipeID)
	{
		for (Recipe* recipe : *recipes)
		{
			if (recipe->getRecipeID() == recipeID)
				return recipe;
		}

		return nullptr;
	}

	void Manager::loadRecipes()
	{
		recipes = new std::vector<Recipe*>();
		recipesIDs = new std::vector<mcString>();

		loadStaticRecipes();
		loadDynamicRecipes();
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
