#ifndef __INCLUDE_CRAFTING_HPP
#define __INCLUDE_CRAFTING_HPP

#include <vector>
#include <initializer_list>

#include "Item.hpp"

namespace rgl {

class RecipeComponent {
public:
	virtual ~RecipeComponent() {
	}
	virtual bool matches(const Item &item) = 0;
};

// A component which only matches a given item
class ItemComponent: public RecipeComponent {
private:
	const Item item;

public:
	explicit ItemComponent(const Item &item) :
			item { item } {
	}

	bool matches(const Item &item) override {
		return item.name == this->item.name;
	}
};

class ItemTypeComponent: public RecipeComponent {
private:
	const ItemType type;

public:
	explicit ItemTypeComponent(const ItemType itemType) :
			type { itemType } {
	}

	bool matches(const Item &item) override {
		return item.type == this->type;
	}
};

class CraftingRecipe {
public:
	const Item targetItem;
	const std::vector<RecipeComponent> components;

	explicit CraftingRecipe(std::initializer_list<RecipeComponent> components, Item &&targetItem) :
			components { components }, targetItem { targetItem } {
	}
};

class CraftingManager {
private:
	std::vector<CraftingRecipe> recipes;

public:
	explicit CraftingManager() {
	}

	inline void addRecipe(CraftingRecipe &&recipe) {
		recipes.emplace_back(std::move(recipe));
	}
};

}

#endif

