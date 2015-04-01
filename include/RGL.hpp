/*
 * RGL.hpp
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

#ifndef INCLUDE_RGL_HPP_
#define INCLUDE_RGL_HPP_

#include <string>

#include <Ashley/core/Engine.hpp>

#include "systems/MapRenderSystem.hpp"
#include "systems/RenderSystem.hpp"

#include "Map.hpp"
#include "Message.hpp"

namespace rgl {

class RGL {
private:
	const std::string windowTitle;

	ashley::Engine engine;

	ashley::Entity *player = nullptr, *mapComponent = nullptr;
	MapRenderSystem * mapRenderSystem = nullptr;
	RenderSystem * renderSystem = nullptr;

	Map map;

	MessageHandler messageHandler;

	void renderHUD();

public:
	static const int CONSOLE_WIDTH = 100;
	static const int CONSOLE_HEIGHT = 55;
	static const int STATUS_BAR_HEIGHT = 5;

	explicit RGL(const std::string &windowTitle) :
			windowTitle { windowTitle }, //
			engine { }, //
			map { CONSOLE_WIDTH, CONSOLE_HEIGHT - STATUS_BAR_HEIGHT - 1 }, //
			messageHandler { nullptr, CONSOLE_WIDTH / 2, CONSOLE_HEIGHT - 6, 6 } //
	{
		TCODConsole::initRoot(CONSOLE_WIDTH, CONSOLE_HEIGHT, windowTitle.c_str(), false, TCOD_RENDERER_GLSL);
		messageHandler.setConsole(TCODConsole::root);
	}

	void init();
	void update(float deltaTime);

	ashley::Entity * getPlayer() const {
		return player;
	}
};

}

#endif /* INCLUDE_RGL_HPP_ */
