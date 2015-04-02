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
#include "Item.hpp"
#include "Stats.hpp"

#include "components/Position.hpp"
#include "components/Renderable.hpp"
#include "components/PlayerInputListener.hpp"
#include "components/MapRenderable.hpp"
#include "components/Interactible.hpp"
#include "components/Inventory.hpp"

#include "systems/MapRenderSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/PlayerInputSystem.hpp"

#include "easylogging++.h"

rgl::MessageHandler *rgl::MessageHandler::globalHandler = nullptr;

void rgl::RGL::init() {
	messageHandler.addMessage("Welcome to RGL!");
	MessageHandler::globalHandler = &messageHandler;
	const Room room = map.getRandomRoom();

	std::vector<Room> takenRooms;
	takenRooms.push_back(room);

	const char *names[5] = { "Sword of Yendor", "Amulet of Awesome", "Ion Beam Gun", "Chicken Leg", "Leather Whip" };

	for (int i = 0; i < 5; ++i) {
		Room *chestRoom = nullptr;

		while (true) {
			chestRoom = &(map.getRandomRoom());
			bool populatedRoom = false;

			for (const auto &room : takenRooms) {
				if (*chestRoom == room) {
					populatedRoom = true;
					break;
				}
			}

			if (!populatedRoom) {
				takenRooms.push_back(*chestRoom);
				break;
			}
		}

		auto chest = engine.addEntity();
		const auto xNumerator = TCODRandom::getInstance()->getInt(1, 3);
		const auto yNumerator = TCODRandom::getInstance()->getInt(1, 3);

		const auto chestX = chestRoom->x1 + xNumerator * (chestRoom->w / 4);
		const auto chestY = chestRoom->y1 + yNumerator * (chestRoom->h / 4);

		if (chestX == 0 && chestY == 0) {
			RGLL->debug("Invalid chest generated at (%v, %v).", chestX, chestY);
		}

		chest->add<Position>(chestX, chestY);
		chest->add<Renderable>('#', TCODColor::yellow);
		chest->add<Interactible>(InteractionType::LOOT);
		chest->add<Inventory>();
		ashley::ComponentMapper<Inventory>::getMapper().get(chest)->contents.emplace_back(
		        Item(names[i], ItemType::WEAPON, Stats(50, 10, 5)));

		const auto pos = chest->getComponent<Position>();
		map.getTileAt(pos->position.x, pos->position.y)->contains.emplace_back(chest);
	}

	const auto tiger = engine.addEntity();
	tiger->add<Position>(map.getRoom(0).x1 + 1, map.getRoom(0).y1 + 1);
	tiger->add<Renderable>('T', TCODColor::orange);
	tiger->add<Inventory>(Item("Tiger Skin", ItemType::CRAFTING, CraftingGroup::SKIN));
	tiger->add<Interactible>(InteractionType::FIGHT);

	player = engine.addEntity();
	player->add<Position>(room.x1 + (room.w / 2), room.y1 + (room.h / 2));
	player->add<Renderable>('@', TCODColor::red);
	player->add<PlayerInputListener>();
	player->add<Inventory>();

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
	renderHUD();
	messageHandler.render();
	TCODConsole::root->flush();
}

void rgl::RGL::renderHUD() {
	const auto con = TCODConsole::root;
	static const int msgX = 1;

	con->print(msgX, CONSOLE_HEIGHT - 6, "@ the Fearless");
	con->setCharForeground(msgX, CONSOLE_HEIGHT - 6, TCODColor::red);
	con->print(msgX, CONSOLE_HEIGHT - 5, "HP: 10/10");
	con->print(msgX, CONSOLE_HEIGHT - 3, "Naked");
}
