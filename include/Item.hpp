/*
 * Item.hpp
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

#ifndef INCLUDE_ITEM_HPP_
#define INCLUDE_ITEM_HPP_

#include <string>
#include <vector>

#include "Stats.hpp"

namespace rgl {

enum class ItemType {
	WEAPON, // equippable as a weapon
	ARMOR, // equippable as armor
	CONSUMABLE, // consumable with some effect, maybe limited use
	CRAFTING  // used for crafting (note that weapons and armor may also be used for crafting)
};

enum class CraftingGroup {
	WEAPON, // already a weapon, but maybe usable in some recipes
	ARMOR, // already armor but maybe usuable in some recipes
	METAL, // a piece of metal
	MAGIC, // some sort of magical component
	SKIN, // skin from an animal
	NONE // not usable in crafting
};

class Augment {
public:
	const std::string name;
	const Stats effect;

	explicit Augment(const std::string &name, const Stats &effect) :
			name { name }, effect { effect } {
	}
};

class Item {
private:
	void deduceCraftingGroup();
	std::vector<Augment> augments;

public:
	explicit Item(const std::string &name, ItemType type, CraftingGroup group);
	explicit Item(const std::string &name, ItemType type, Stats stats, CraftingGroup group = CraftingGroup::NONE);

	std::string name;

	ItemType type;

	Stats stats; // for weapons, armor and consumables
	CraftingGroup group; // for crafting

	bool isAugmented() const {
		return augments.size() > 0;
	}

	inline void applyAugment(const Augment &augment) {
		augments.push_back(augment);
		stats += augment.effect;
	}

	std::vector<Augment> &getAugments() {
		return augments;
	}
};

}

#endif /* INCLUDE_ITEM_HPP_ */
