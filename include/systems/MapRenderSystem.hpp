#ifndef INCLUDE_SYSTEMS_MAPRENDERSYSTEM_HPP_
#define INCLUDE_SYSTEMS_MAPRENDERSYSTEM_HPP_

#include <Ashley/systems/IteratingSystem.hpp>
#include <components/MapRenderable.hpp>

#include "components/Position.hpp"

namespace rgl {

class MapRenderSystem: public ashley::IteratingSystem {
private:
	TCODConsole * const console = nullptr;

public:
	explicit MapRenderSystem(TCODConsole * console, int priority) :
			IteratingSystem(ashley::Family::getFor( { typeid(Position), typeid(MapRenderable) }), priority), //
			console { console } {
	}

	void processEntity(ashley::Entity * const entity, float deltaTime) override;
};

}

#endif /* INCLUDE_SYSTEMS_MAPRENDERSYSTEM_HPP_ */
