/*
 * Crafting.hpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ashley Davis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

