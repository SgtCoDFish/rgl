/*
 * Position.hpp
 *
 *  Created on: 29 Mar 2015
 *  Author: Ashley Davis (SgtCoDFish)
 */

#ifndef INCLUDE_COMPONENTS_POSITION_HPP_
#define INCLUDE_COMPONENTS_POSITION_HPP_

#include <Ashley/core/Component.hpp>

#include <glm/vec2.hpp>

namespace rgl {

class Position: public ashley::Component {
public:
	Position() :
			position { 0, 0 } {
	}

	Position(int x, int y) :
			position { x, y } {
	}

	glm::ivec2 position {0, 0};
};

}

#endif /* INCLUDE_COMPONENTS_POSITION_HPP_ */
