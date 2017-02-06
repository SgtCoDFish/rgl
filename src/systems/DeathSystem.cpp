#include <string>

#include <Ashley/core/ComponentMapper.hpp>

#include "systems/DeathSystem.hpp"

#include "Message.hpp"

void rgl::DeathSystem::processEntity(ashley::Entity * const entity, float deltaTime) {
	MessageHandler::globalHandler->addDeathMessage(entity);

	engine->removeEntity(entity);
}
