#ifndef INCLUDE_COMPONENTS_MAPRENDERABLE_HPP_
#define INCLUDE_COMPONENTS_MAPRENDERABLE_HPP_

#include <Ashley/core/Component.hpp>

#include "libtcod.hpp"

#include "Map.hpp"

namespace rgl {

class MapRenderable: public ashley::Component {
public:
	const Map &map;

	const TCODColor wallColor;
	const TCODColor groundColor;

	explicit MapRenderable(const Map &map) : MapRenderable(map, TCODColor::grey, TCODColor::black) {}

	explicit MapRenderable(const Map &map, const TCODColor &wallColor, const TCODColor &groundColor) :
			map(map), wallColor(wallColor), groundColor(groundColor) {
	}
};

}

#endif /* INCLUDE_COMPONENTS_MAPRENDERABLE_HPP_ */
