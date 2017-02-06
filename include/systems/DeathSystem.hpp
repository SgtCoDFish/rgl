#ifndef INCLUDE_SYSTEMS_DEATHSYSTEM_HPP_
#define INCLUDE_SYSTEMS_DEATHSYSTEM_HPP_

#include <typeinfo>

#include <Ashley/systems/IteratingSystem.hpp>
#include <Ashley/core/Family.hpp>
#include <Ashley/core/Entity.hpp>
#include <Ashley/core/Engine.hpp>

#include "components/DeathMarker.hpp"

namespace rgl {

class DeathSystem: public ashley::IteratingSystem {
private:
	ashley::Engine *engine = nullptr;

public:
	explicit DeathSystem(int priority) :
			IteratingSystem(ashley::Family::getFor( { typeid(DeathMarker) }), priority) {
	}

	void addedToEngine(ashley::Engine &engine) override {
		IteratingSystem::addedToEngine(engine);
		this->engine = &engine;
	}

	void removedFromEngine(ashley::Engine &engine) override {
		IteratingSystem::removedFromEngine(engine);
		this->engine = nullptr;
	}

	void processEntity(ashley::Entity * const, float) override;
};

}

#endif /* INCLUDE_SYSTEMS_DEATHSYSTEM_HPP_ */
