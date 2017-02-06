#include <Ashley/core/Entity.hpp>
#include <Ashley/core/Engine.hpp>

#include "libtcod.hpp"

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
