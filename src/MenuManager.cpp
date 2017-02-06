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

#include <Ashley/Ashley.hpp>

#include "libtcod.hpp"

#include "components/Inventory.hpp"
#include "components/Named.hpp"
#include "components/Battling.hpp"

#include "Stats.hpp"

#include "MenuManager.hpp"

#include "easylogging++.h"

rgl::Menu::Menu(const std::string &title, int w, int h, MenuType type, ashley::Entity * const entity) :
		title { title }, type { type }, width { w }, height { h }, entity { entity } {
	console = std::make_unique<TCODConsole>(w, h);
}

void rgl::Menu::render(TCODConsole *target, int x, int y) {
	switch (type) {
	case MenuType::INVENTORY: {
		renderInventory(target, x, y);
		break;
	}

	case MenuType::PLAYER_DETAILS: {
		renderPlayerDetails(target, x, y);
		break;
	}

	case MenuType::ITEM_DETAILS: {
		RGLL->debug("Item details menu NYI.");
		break;
	}

	case MenuType::EQUIPMENT: {
		RGLL->debug("Equipment menu NYI.");
		break;
	}

	case MenuType::YES_NO: {
		renderYesNo(target, x, y);
		break;
	}

	default:
	break;
}
}

void rgl::Menu::renderInventory(TCODConsole *target, int targetX, int targetY) {
	console->setDefaultBackground(TCODColor::darkestGrey);
	console->clear();

	renderRect(console.get(), 1, 3, console->getWidth() - 3, console->getHeight() - 5);

	const auto inventory = ashley::ComponentMapper<Inventory>::getMapper().get(entity);

	const int titleX = std::max((int) (console->getWidth() - title.length()) / 2, 0);

	console->print(titleX, 1, "%s", title.c_str());

	if (inventory == nullptr) {
		console->print(3, 5, "Couldn't find an inventory.");
		return;
	}

	if (inventory->contents.empty()) {
		console->print(3, 5, "Empty");
	} else {
		for (int i = 0; i < (int) inventory->contents.size(); ++i) {
			const auto &item = inventory->contents[i];
			const char *augmentString = (item.isAugmented() ? " *" : "");

			console->print(3, 5 + i, "%d: %s%s", i + 1, item.name.c_str(), augmentString);
		}
	}

	TCODConsole::blit(console.get(), 0, 0, console->getWidth(), console->getHeight(), target, targetX, targetY);
}

void rgl::Menu::renderPlayerDetails(TCODConsole *target, int targetX, int targetY) {
	console->setDefaultBackground(TCODColor::darkestGrey);
	console->clear();

	renderRect(console.get(), 1, 3, console->getWidth() - 3, console->getHeight() - 5);

	const int titleX = std::max((int) (console->getWidth() - title.length()) / 2, 0);
	console->print(titleX, 1, "%s", title.c_str());

	const auto battling = ashley::ComponentMapper<Battling>::getMapper().get(entity);
	const auto named = ashley::ComponentMapper<Named>::getMapper().get(entity);

	if (battling == nullptr) {
		console->print(3, 5, "Couldn't find player details.");
		return;
	}

	const auto &stats = battling->stats;

	const char *name = (named == nullptr ? "Unknown" : named->name.c_str());

	const int baseX = 3;
	const int baseY = 5;

	console->print(baseX, baseY, "%s the Valiant", name);
	console->print(baseX, baseY + 2, "Max HP: %d", stats.maxHP);

	console->print(baseX, baseY + 4, "Atk: %d", stats.atk);
	console->print(baseX, baseY + 5, "Def: %d", stats.def);

	console->print(baseX, baseY + 7, "Spd: %d", stats.speed);

	TCODConsole::blit(console.get(), 0, 0, console->getWidth(), console->getHeight(), target, targetX, targetY);
}

void rgl::Menu::renderYesNo(TCODConsole *target, int targetX, int targetY) {
	if((int)title.length() > console->getWidth()) {
		// split
	}

	console->print(0, 0, "%s", title.c_str());

	TCODConsole::blit(console.get(), 0, 0, console->getWidth(), console->getHeight(), target, targetX, targetY);
}

void rgl::Menu::renderRect(TCODConsole *console, int x, int y, int w, int h) {
	console->hline(x, y, w + 1);
	console->hline(x, y + h, w + 1);
	console->vline(x, y + 1, h - 1);
	console->vline(x + w, y + 1, h - 1);

	const TCODColor currBack = console->getDefaultBackground();
	console->setDefaultBackground(TCODColor::darkGrey);
	console->rect(x + 1, y + 1, w - 1, h - 1, false, TCOD_BKGND_SET);

	console->setDefaultBackground(currBack);
}

rgl::MenuManager::MenuManager(int maxWidth, int maxHeight) :
		maxWidth { maxWidth }, maxHeight { maxHeight } {
}

void rgl::MenuManager::render(TCODConsole *target) {
	for (auto &menu : menus) {
		const int x = (maxWidth - menu.width) / 2;
		const int y = (maxHeight - menu.height) / 2;

		menu.render(target, x, y);
	}
}

void rgl::MenuManager::pushMenu(Menu &&menu) {
	menus.emplace_back(std::move(menu));
}

void rgl::MenuManager::pushPlayerMenu(ashley::Entity *entity) {
	const auto name = ashley::ComponentMapper<Named>::getMapper().get(entity)->name;

	menus.emplace_back(name, maxWidth / 3, maxHeight / 2, MenuType::PLAYER_DETAILS, entity);
}

void rgl::MenuManager::pushInventoryMenu(ashley::Entity * entity) {
	const auto nameStr = ashley::ComponentMapper<Named>::getMapper().get(entity)->name + "'s Inventory";

	menus.emplace_back(nameStr, maxWidth / 3, maxHeight / 2, MenuType::INVENTORY, entity);
}

void rgl::MenuManager::pushYesNoMenu(const std::string &question) {
	menus.emplace_back(question, maxWidth / 3, maxHeight / 2, MenuType::YES_NO, nullptr);
}

bool rgl::MenuManager::popMenu() {
	if (menus.size() > 0u) {
		menus.pop_back();
	}

	return !hasActiveMenus();
}

const rgl::Menu *rgl::MenuManager::getTopMenu() const {
	return &(menus.back());
}

bool rgl::MenuManager::hasActiveMenus() const {
	return menus.size() > 0;
}

int rgl::MenuManager::numActiveMenus() const {
	return menus.size();
}

