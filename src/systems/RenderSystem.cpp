#include <Ashley/Ashley.hpp>

#include "systems/RenderSystem.hpp"

void rgl::RenderSystem::processEntity(ashley::Entity * const entity, float deltaTime) {
	const auto pos = ashley::ComponentMapper<Position>::getMapper().get(entity);
	const auto renderable = ashley::ComponentMapper<Renderable>::getMapper().get(entity);

	console->putChar(pos->position.x, pos->position.y, renderable->c);
	console->setCharForeground(pos->position.x, pos->position.y, renderable->color);
}
