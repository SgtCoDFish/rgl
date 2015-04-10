/*
 * EntityUtil.cpp
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

#include <Ashley/core/Entity.hpp>
#include <Ashley/core/Engine.hpp>

#include "tcod/libtcod.hpp"

#include "components/Position.hpp"
#include "components/Renderable.hpp"
#include "components/PlayerInputListener.hpp"
#include "components/MapRenderable.hpp"
#include "components/Interactible.hpp"
#include "components/Inventory.hpp"
#include "components/Battling.hpp"
#include "components/Named.hpp"

#include "EntityUtil.hpp"
#include "Item.hpp"

ashley::Entity *rgl::EntityUtil::generateTigerEntity(ashley::Engine &engine, int x, int y) {
	const auto e = engine.addEntity();

	e->add<Position>(x, y);
	e->add<Renderable>('T', TCODColor::orange);
	e->add<Inventory>(Item("Tiger Skin", ItemType::CRAFTING, CraftingGroup::SKIN));
	e->add<Interactible>(InteractionType::FIGHT);
	e->add<Battling>(Stats(4, 2, 1, 1));
	e->add<Named>("tiger", NameType::SPECIES);

	return e;
}

ashley::Entity *rgl::EntityUtil::generateSabretoothEntity(ashley::Engine &engine, int x, int y) {
	const auto e = engine.addEntity();

	e->add<Position>(x, y);
	e->add<Renderable>('T', TCODColor::red);
	e->add<Inventory>(Item("Sabretooth Skin", ItemType::CRAFTING, CraftingGroup::SKIN));
	e->add<Interactible>(InteractionType::FIGHT);
	e->add<Battling>(Stats(10, 2, 0, 2));
	e->add<Named>("sabretooth", NameType::SPECIES);

	return e;
}

ashley::Entity *rgl::EntityUtil::generateChestEntity(ashley::Engine &engine, int x, int y, const Item &item) {
	const auto e = engine.addEntity();

	e->add<Position>(x, y);
	e->add<Renderable>('#', TCODColor::copper);
	e->add<Interactible>(InteractionType::LOOT_CHEST);
	e->add<Inventory>();
	ashley::ComponentMapper<Inventory>::getMapper().get(e)->contents.emplace_back(item);

	return e;
}
