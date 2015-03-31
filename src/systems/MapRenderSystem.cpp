/*
 * MapRenderSystem.cpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ashley Davis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <Ashley/AshleyCore.hpp>

#include "systems/MapRenderSystem.hpp"

void rgl::MapRenderSystem::processEntity(ashley::Entity * const &entity, float deltaTime) {
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
