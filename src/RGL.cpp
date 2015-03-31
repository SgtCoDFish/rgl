/*
 * RGL.cpp
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

#include "libtcod.hpp"

#include "RGL.hpp"

#include "components/Position.hpp"
#include "components/Renderable.hpp"
#include "components/PlayerInputListener.hpp"
#include "components/MapRenderable.hpp"

#include "systems/MapRenderSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/PlayerInputSystem.hpp"

void rgl::RGL::init() {
	TCODConsole::initRoot(CONSOLE_WIDTH, CONSOLE_HEIGHT, windowTitle.c_str(), false, TCOD_RENDERER_GLSL);

	const Room room = map.getRandomRoom();

	player = engine.addEntity();
	player->add<Position>(room.x1 + (room.w / 2), room.y1 + (room.h / 2));
	player->add<Renderable>('@', TCODColor::red);
	player->add<PlayerInputListener>();

	mapComponent = engine.addEntity();
	mapComponent->add<Position>(0, 0);
	mapComponent->add<MapRenderable>(map);

	engine.addSystem<PlayerInputSystem>(&map);
	mapRenderSystem = engine.addSystem<MapRenderSystem>(TCODConsole::root);
	renderSystem = engine.addSystem<RenderSystem>(TCODConsole::root);
}

void rgl::RGL::update(float deltaTime) {
	TCODConsole::root->clear();
	engine.update(deltaTime);
	TCODConsole::root->flush();
}
