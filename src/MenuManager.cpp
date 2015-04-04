/*
 * MenuManager.cpp
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

#include <memory>
#include <algorithm>

#include <Ashley/AshleyCore.hpp>

#include "libtcod.hpp"

#include "components/Inventory.hpp"
#include "components/Named.hpp"

#include "MenuManager.hpp"

rgl::MenuManager::MenuManager(int maxWidth, int maxHeight) :
		maxWidth { maxWidth }, maxHeight { maxHeight } {
	inventoryConsole = std::make_unique<TCODConsole>(maxWidth / 3, maxHeight / 2);
}

void rgl::MenuManager::renderInventory(ashley::Entity *entity, TCODConsole *target, int targetX, int targetY) {
	inventoryConsole->setDefaultBackground(TCODColor::black);
	inventoryConsole->clear();

	renderRect(inventoryConsole.get(), 1, 3, inventoryConsole->getWidth() - 3, inventoryConsole->getHeight() - 5);

	const auto inventory = ashley::ComponentMapper<Inventory>::getMapper().get(entity);

	const auto nameStr = ashley::ComponentMapper<Named>::getMapper().get(entity)->name + "'s Inventory";
	const int nameX = std::max((int) (inventoryConsole->getWidth() - nameStr.length()) / 2, 0);

	inventoryConsole->print(nameX, 1, "%s", nameStr.c_str());

	for (int i = 0; i < (int) inventory->contents.size(); ++i) {
		inventoryConsole->print(3, 5 + i, "%d: %s", i + 1, inventory->contents[i].name.c_str());
	}

	TCODConsole::blit(inventoryConsole.get(), 0, 0, inventoryConsole->getWidth(), inventoryConsole->getHeight(), target,
	        targetX, targetY);
}

void rgl::MenuManager::renderRect(TCODConsole *console, int x, int y, int w, int h) {
	console->hline(x, y, w + 1);
	console->hline(x, y + h, w + 1);
	console->vline(x, y + 1, h - 1);
	console->vline(x + w, y + 1, h - 1);

	const TCODColor currBack = console->getDefaultBackground();
	console->setDefaultBackground(TCODColor::darkGrey);
	console->rect(x + 1, y + 1, w - 1, h - 1, false, TCOD_BKGND_SET);

	console->setDefaultBackground(currBack);
}
