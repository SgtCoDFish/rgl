#include <Ashley/Ashley.hpp>

#include "libtcod.hpp"

#include "RGL.hpp"
#include "EntityUtil.hpp"
#include "Item.hpp"
#include "Stats.hpp"

#include "components/Position.hpp"
#include "components/Renderable.hpp"
#include "components/PlayerInputListener.hpp"
#include "components/MapRenderable.hpp"
#include "components/Interactible.hpp"
#include "components/Inventory.hpp"
#include "components/Battling.hpp"
#include "components/Named.hpp"

#include "systems/MapRenderSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/PlayerInputSystem.hpp"
#include "systems/BattleSystem.hpp"
#include "systems/DeathSystem.hpp"

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

		const auto xNumerator = TCODRandom::getInstance()->getInt(1, 3);
		const auto yNumerator = TCODRandom::getInstance()->getInt(1, 3);

		const auto chestX = chestRoom->x1 + xNumerator * (chestRoom->w / 4);
		const auto chestY = chestRoom->y1 + yNumerator * (chestRoom->h / 4);

		if (chestX == 0 && chestY == 0) {
			RGLL->debug("Invalid chest generated at (%v, %v).", chestX, chestY);
		}

		auto item = Item(names[i], ItemType::WEAPON, Stats(10, 5 - i + 1, 2));

		if (i == 0) {
			item.applyAugment(Augment("Yendor's Boon", Stats(1, 2, 3, 0)));
		}

		auto chest = EntityUtil::generateChestEntity(engine, chestX, chestY, item);

		map.registerTileContents(chest);
	}

	map.registerTileContents(EntityUtil::generateTigerEntity(engine, room.x1 + 1, room.y1 + 1));
	map.registerTileContents(EntityUtil::generateSabretoothEntity(engine, room.x2 - 1, room.y2 - 1));

	player = engine.addEntity();
	player->add<Position>(room.x1 + (room.w / 2), room.y1 + (room.h / 2));
	player->add<Renderable>('@', TCODColor::red);
	player->add<PlayerInputListener>();
	player->add<Inventory>();
	player->add<Battling>(Stats(10, 3, 1, 2));
	player->add<Named>("@");

	mapComponent = engine.addEntity();
	mapComponent->add<Position>(0, 0);
	mapComponent->add<MapRenderable>(map);

	battleSystem = engine.addSystem<BattleSystem>(1000);
	engine.addSystem<PlayerInputSystem>(&map, battleSystem, &menuManager, 0);
	engine.addSystem<DeathSystem>(5000);
	mapRenderSystem = engine.addSystem<MapRenderSystem>(TCODConsole::root, 100000);
	renderSystem = engine.addSystem<RenderSystem>(TCODConsole::root, 500000);
}

void rgl::RGL::update(float deltaTime) {
	TCODConsole::root->clear();
	engine.update(deltaTime);
	renderHUD();
	messageHandler.render();
	menuManager.render(TCODConsole::root);
	TCODConsole::root->flush();
}

void rgl::RGL::renderHUD() {
	const auto con = TCODConsole::root;
	static const int msgX = 1;

	const auto &playerName = ashley::ComponentMapper<Named>::getMapper().get(player)->name;
	const auto playerStats = ashley::ComponentMapper<Battling>::getMapper().get(player)->stats;

	con->print(msgX, CONSOLE_HEIGHT - 6, "%s the Fearless", playerName.c_str());
	for (unsigned int i = 0u; i < playerName.length(); ++i) {
		con->setCharForeground(msgX + i, CONSOLE_HEIGHT - 6, TCODColor::red);
	}

	con->print(msgX, CONSOLE_HEIGHT - 5, "HP: %d/%d", playerStats.hp, playerStats.maxHP);
	con->print(msgX, CONSOLE_HEIGHT - 3, "Naked");
}
