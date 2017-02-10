#include <iostream>

#include "Item.hpp"

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
	}
}
