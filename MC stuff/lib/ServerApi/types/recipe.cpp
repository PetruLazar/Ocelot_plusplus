
#include "recipe.h"

namespace recipe
{
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
