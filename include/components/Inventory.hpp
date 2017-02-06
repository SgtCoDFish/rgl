#ifndef INCLUDE_COMPONENTS_INVENTORY_HPP_
#define INCLUDE_COMPONENTS_INVENTORY_HPP_

#include <string>
#include <vector>

#include <Ashley/core/Component.hpp>

#include "Item.hpp"

namespace rgl {

class Inventory: public ashley::Component {
public:
	std::vector<Item> contents;

	explicit Inventory() {
	}

	explicit Inventory(const Item &item) {
		contents.emplace_back(item);
	}
};

}

#endif /* INCLUDE_COMPONENTS_INVENTORY_HPP_ */
