#include <Ashley/Ashley.hpp>

#include "systems/MapRenderSystem.hpp"

void rgl::MapRenderSystem::processEntity(ashley::Entity * const entity, float deltaTime) {
	const auto mapComponent = ashley::ComponentMapper<MapRenderable>::getMapper().get(entity);

	for (int y = 0; y < mapComponent->map.getHeight(); ++y) {
		for (int x = 0; x < mapComponent->map.getWidth(); ++x) {
			const auto tile = mapComponent->map.getTileAt(x, y);

			if (tile != nullptr) {
				console->setCharBackground(x, y, tile->solid ? mapComponent->wallColor : mapComponent->groundColor);
			}
		}
	}
}
