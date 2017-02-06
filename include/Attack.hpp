#ifndef INCLUDE_ATTACK_HPP_
#define INCLUDE_ATTACK_HPP_

#include <Ashley/core/Entity.hpp>

#include "components/Battling.hpp"

#include "Stats.hpp"

namespace rgl {

class Attack {
public:
	ashley::Entity * const from;
	ashley::Entity * const against;

	explicit Attack(ashley::Entity * const from, ashley::Entity * const against) :
			from { from }, against { against } {
	}
};

}

#endif /* INCLUDE_ATTACK_HPP_ */
