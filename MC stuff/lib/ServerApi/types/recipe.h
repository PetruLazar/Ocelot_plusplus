#pragma once
#include "varData.h"
#include "../types.h"

namespace recipe
{
	struct Ingredient
	{
		varInt count;
		Slot** items;

		Ingredient(varInt count, Slot** items) : count(count), items(items) {}

		~Ingredient() {
			for (int i = 0; i < count; i++)
				delete items[i];

			delete[] items;
		}

		void write(char*& buffer) const;
	};

	class Recipe
	{
	protected:
		mcString type, recipeID;

	public:
		Recipe(mcString type, mcString recipeID) : type(type), recipeID(recipeID) {}

		static const std::string crafting_shapeless, crafting_shaped, smelting, blasting, smoking, campfire_cooking, stonecutting, smithing,
			crafting_special_armordye, crafting_special_bookcloning, crafting_special_mapcloning, crafting_special_mapextending, crafting_special_firework_rocket,
			crafting_special_firework_star, crafting_special_firework_star_fade, crafting_special_repairitem, crafting_special_tippedarrow, crafting_special_bannerduplicate,
			crafting_special_banneraddpattern, crafting_special_shielddecoration, crafting_special_shulkerboxcoloring, crafting_special_suspiciousstew;
		
		virtual void write(char*& buffer) const;

		const mcString& getType() { return type; }
		const mcString& getRecipeID() { return recipeID; }
	};

	class Manager
	{
	private:
		static Ingredient* getItemOrTag(json* data);

		static void addRecipe(const mcString& id, recipe::Recipe* recipe);
		static void loadStaticRecipes();
		static void loadDynamicRecipes();

	public:
		static std::vector<Recipe*>* recipes;
		static std::vector<mcString>* recipesIDs;

		SERVER_API static const Recipe* getRecipe(const std::string& recipeID);

		SERVER_API static void loadRecipes();
		SERVER_API static void unloadRecipes();
	};
	
	class crafting_shapeless : public Recipe
	{
	public:
		mcString group;
		varInt ingredientCount;
		Ingredient** ingredients;
		Slot* result;

		crafting_shapeless(mcString recipeID, mcString group, varInt ingredientCount, Ingredient** ingredients, Slot* result)
			: group(group), ingredientCount(ingredientCount), ingredients(ingredients), result(result), Recipe("crafting_shapeless", recipeID) {}

		void write(char*& buffer) const;
	};

	class crafting_shaped : public Recipe
	{
	public:
		varInt width, height;
		mcString group;
		Ingredient** ingredients;
		Slot* result;

		crafting_shaped(mcString recipeID, varInt width, varInt height, mcString group, Ingredient** ingredients, Slot* result) 
			: width(width), height(height), group(group), ingredients(ingredients), result(result), Recipe("crafting_shaped", recipeID) {}

		void write(char*& buffer) const;
	};

	struct crafting_special_armordye : public Recipe
	{
		crafting_special_armordye(mcString recipeID) 
			: Recipe("crafting_special_armordye", recipeID) {}
	};

	struct crafting_special_bookcloning : public Recipe
	{
		crafting_special_bookcloning(mcString recipeID) 
			: Recipe("crafting_special_bookcloning", recipeID) {}
	};

	struct crafting_special_mapcloning : public Recipe
	{
		crafting_special_mapcloning(mcString recipeID) 
			: Recipe("crafting_special_mapcloning", recipeID) {}
	};

	struct crafting_special_mapextending : public Recipe
	{
		crafting_special_mapextending(mcString recipeID) 
			: Recipe("crafting_special_mapextending", recipeID) {}
	};

	struct crafting_special_firework_rocket : public Recipe
	{
		crafting_special_firework_rocket(mcString recipeID) 
			: Recipe("crafting_special_firework_rocket", recipeID) {}
	};

	struct crafting_special_firework_star : public Recipe
	{
		crafting_special_firework_star(mcString recipeID) 
			: Recipe("crafting_special_firework_star", recipeID) {}
	};

	struct crafting_special_firework_star_fade : public Recipe
	{
		crafting_special_firework_star_fade(mcString recipeID) 
			: Recipe("crafting_special_firework_star_fade", recipeID) {}
	};

	struct crafting_special_repairitem : public Recipe
	{
		crafting_special_repairitem(mcString recipeID) 
			: Recipe("crafting_special_repairitem", recipeID) {}
	};

	struct crafting_special_tippedarrow : public Recipe
	{
		crafting_special_tippedarrow(mcString recipeID) 
			: Recipe("crafting_special_tippedarrow", recipeID) {}
	};

	struct crafting_special_bannerduplicate : public Recipe
	{
		crafting_special_bannerduplicate(mcString recipeID) 
			: Recipe("crafting_special_bannerduplicate", recipeID) {}
	};

	struct crafting_special_banneraddpattern : public Recipe
	{
		crafting_special_banneraddpattern(mcString recipeID) 
			: Recipe("crafting_special_banneraddpattern", recipeID) {}
	};

	struct crafting_special_shielddecoration : public Recipe
	{
		crafting_special_shielddecoration(mcString recipeID) 
			: Recipe("crafting_special_shielddecoration", recipeID) {}
	};

	struct crafting_special_shulkerboxcoloring : public Recipe
	{
		crafting_special_shulkerboxcoloring(mcString recipeID) 
			: Recipe("crafting_special_shulkerboxcoloring", recipeID) {}
	};

	struct crafting_special_suspiciousstew : public Recipe
	{
		crafting_special_suspiciousstew(mcString recipeID) 
			: Recipe("crafting_special_suspiciousstew", recipeID) {}
	};

	class smelting : public Recipe
	{
	public:
		mcString group;
		Ingredient* ingredient;
		Slot* result;
		bfloat experience;
		varInt cookingTime;

		smelting(mcString recipeID, mcString group, Ingredient* ingredient, Slot* result, bfloat experience, varInt cookingTime)
			: group(group), ingredient(ingredient), result(result), experience(experience), cookingTime(cookingTime), Recipe("smelting", recipeID) {}

		void write(char*& buffer) const;
	};

	class blasting : public Recipe
	{
	public:
		mcString group;
		Ingredient* ingredient;
		Slot* result;
		bfloat experience;
		varInt cookingTime;

		blasting(mcString recipeID, mcString group, Ingredient* ingredient, Slot* result, bfloat experience, varInt cookingTime) 
			: group(group), ingredient(ingredient), result(result), experience(experience), cookingTime(cookingTime), Recipe("blasting", recipeID) {}

		void write(char*& buffer) const;
	};

	class smoking : public Recipe
	{
	public:
		mcString group;
		Ingredient* ingredient;
		Slot* result;
		bfloat experience;
		varInt cookingTime;

		smoking(mcString recipeID, mcString group, Ingredient* ingredient, Slot* result, bfloat experience, varInt cookingTime) 
			: group(group), ingredient(ingredient), result(result), experience(experience), cookingTime(cookingTime), Recipe("smoking", recipeID) {}

		void write(char*& buffer) const;
	};

	class campfire_cooking : public Recipe
	{
	public:
		mcString group;
		Ingredient* ingredient;
		Slot* result;
		bfloat experience;
		varInt cookingTime;

		campfire_cooking(mcString recipeID, mcString group, Ingredient* ingredient, Slot* result, bfloat experience, varInt cookingTime) 
			: group(group), ingredient(ingredient), result(result), experience(experience), cookingTime(cookingTime), Recipe("campfire_cooking", recipeID) {}

		void write(char*& buffer) const;
	};

	class stonecutting : public Recipe
	{
	public:
		mcString group;
		Ingredient* ingredient;
		Slot* result;

		stonecutting(mcString recipeID, mcString group, Ingredient* ingredient, Slot* result) 
			: group(group), ingredient(ingredient), result(result), Recipe("stonecutting", recipeID) {}

		void write(char*& buffer) const;
	};

	class smithing : public Recipe
	{
	public:
		Ingredient* base, * addition;
		Slot* result;

		smithing(mcString recipeID, Ingredient* base, Ingredient* addition, Slot* result) 
			: base(base), addition(addition), result(result), Recipe("smithing", recipeID) {}

		void write(char*& buffer) const;
	};
}