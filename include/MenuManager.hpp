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
#include <vector>

#include "libtcod.hpp"

namespace ashley {
class Entity;
}

namespace rgl {
enum class MenuType {
	INVENTORY, PLAYER_DETAILS, EQUIPMENT, ITEM_DETAILS
};

class Menu {
private:
	std::unique_ptr<TCODConsole> console;

	void renderRect(TCODConsole *console, int x, int y, int w, int h);

	void renderInventory(TCODConsole * target, int targetX, int targetY);
	void renderPlayerDetails(TCODConsole *target, int targetX, int targetY);

public:
	const std::string title;
	const MenuType type;

	const int width, height;

	ashley::Entity * const entity;

	explicit Menu(const std::string &title, int w, int h, const MenuType type, ashley::Entity * const entity);

	void render(TCODConsole *target, int x, int y);
};

class MenuManager {
private:
	std::vector<Menu> menus;

public:
	const int maxWidth, maxHeight;

	explicit MenuManager(int maxWidth, int maxHeight);

	void render(TCODConsole *target);

	void pushMenu(Menu &&menu);

	void pushPlayerMenu(ashley::Entity *entity);
	void pushInventoryMenu(ashley::Entity * entity);

	// return true if no menus left.
	bool popMenu();

	const Menu * getTopMenu() const;

	inline bool hasActiveMenus() const;
	inline int numActiveMenus() const;
};

}

#endif /* INCLUDE_MENUMANAGER_HPP_ */
