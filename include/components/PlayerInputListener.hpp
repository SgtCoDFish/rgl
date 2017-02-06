#ifndef INCLUDE_COMPONENTS_PLAYERINPUTLISTENER_HPP_
#define INCLUDE_COMPONENTS_PLAYERINPUTLISTENER_HPP_

#include <Ashley/core/Component.hpp>

namespace rgl {

enum class PlayerInputState {
	NORMAL, // moving around
	TARGETTING, // choosing a target for spell/interaction
	//CHOOSING, // choosing from a list of options
	RESPONDING // responding to a yes/no question
};

class PlayerInputListener: public ashley::Component {
public:
	PlayerInputState state;
	ashley::Entity *choice = nullptr;
	glm::ivec2 target { -1, -1 };

	explicit PlayerInputListener(PlayerInputState state = PlayerInputState::NORMAL) :
			state { state } {
	}
};

}

#endif /* INCLUDE_COMPONENTS_PLAYERINPUTLISTENER_HPP_ */
