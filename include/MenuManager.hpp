/*
 * MenuManager.hpp
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

#ifndef INCLUDE_MENUMANAGER_HPP_
#define INCLUDE_MENUMANAGER_HPP_

#include <memory>

#include "libtcod.hpp"

namespace ashley {
class Entity;
}

namespace rgl {
class MenuManager {
private:
	std::unique_ptr<TCODConsole> inventoryConsole;

	void renderRect(TCODConsole *console, int x, int y, int w, int h);

public:
	const int maxWidth, maxHeight;

	explicit MenuManager(int maxWidth, int maxHeight);

	void renderInventory(ashley::Entity *inventory, TCODConsole * target, int targetX, int targetY);
};

}

#endif /* INCLUDE_MENUMANAGER_HPP_ */
