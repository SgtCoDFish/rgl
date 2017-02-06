#ifndef INCLUDE_SYSTEMS_RENDERSYSTEM_HPP_
#define INCLUDE_SYSTEMS_RENDERSYSTEM_HPP_

#include <typeinfo>

#include <Ashley/systems/IteratingSystem.hpp>
#include <Ashley/core/Entity.hpp>
#include <Ashley/core/Family.hpp>

#include "components/Renderable.hpp"
#include "components/Position.hpp"

namespace rgl {

class RenderSystem: public ashley::IteratingSystem {
private:
	TCODConsole * const console;

public:
	explicit RenderSystem(TCODConsole * console, int priority = 500000) :
			IteratingSystem(ashley::Family::getFor( { typeid(Renderable), typeid(Position) }), priority), //
			console { console } {
	}

	virtual void processEntity(ashley::Entity * const entity, float deltaTime) override;
};

}

#endif /* INCLUDE_SYSTEMS_RENDERSYSTEM_HPP_ */
