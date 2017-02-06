#ifndef INCLUDE_COMPONENTS_RENDERABLE_HPP_
#define INCLUDE_COMPONENTS_RENDERABLE_HPP_

#include <Ashley/core/Component.hpp>

#include "libtcod.hpp"

namespace rgl {

class Renderable: public ashley::Component {
public:
	explicit Renderable(char c) :
			Renderable { c, TCODColor::white } {
	}

	explicit Renderable(char c, const TCODColor &color, bool drawLast = false) :
			c { c }, color { color }, drawLast { drawLast } {
	}

	char c;
	TCODColor color;
	bool drawLast;
};

}

#endif /* INCLUDE_COMPONENTS_RENDERABLE_HPP_ */
