/*
 * Item.cpp
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

#include <iostream>

#include "Item.hpp"

#include "easylogging++.h"

rgl::Item::Item(const std::string &name, ItemType type, CraftingGroup group) :
		Item { name, type, Stats(-1, -1, -1), group } {
}

rgl::Item::Item(const std::string &name, ItemType type, Stats stats, CraftingGroup group) :
		name { name }, type { type }, stats { stats }, group { group } {
	deduceCraftingGroup();
}

void rgl::Item::deduceCraftingGroup() {
	if (group == CraftingGroup::NONE) {
		if (type == ItemType::WEAPON) {
			group = CraftingGroup::WEAPON;
		} else if (type == ItemType::ARMOR) {
			group = CraftingGroup::ARMOR;
		}
	} else {
		if (group == CraftingGroup::WEAPON && type != ItemType::WEAPON) {
			RGLL->debug("Trying to create invalid item \"%s\" with WEAPON group and non-WEAPON type.", name.c_str());
		} else if(group == CraftingGroup::ARMOR && type != ItemType::ARMOR) {
			RGLL->debug("Trying to create invalid item \"%s\" with ARMOR group and non-ARMOR type.", name.c_str());
		}
	}
}
