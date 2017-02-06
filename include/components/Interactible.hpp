#ifndef INCLUDE_COMPONENTS_INTERACTIBLE_HPP_
#define INCLUDE_COMPONENTS_INTERACTIBLE_HPP_

#include <Ashley/core/Component.hpp>

namespace rgl {

enum class InteractionType {
	TALK, FIGHT, LOOT_CHEST, LOOT_CORPSE
};

class Interactible: public ashley::Component {
public:
	explicit Interactible(const InteractionType &type) :
			type { type } {
	}

	const InteractionType type;
};

}

#endif /* INCLUDE_COMPONENTS_INTERACTIBLE_HPP_ */
