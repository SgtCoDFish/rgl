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
