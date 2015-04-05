/*
 * BattleSystem.cpp
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

#include <vector>
#include <algorithm>
#include <utility>

#include <Ashley/core/Entity.hpp>
#include <Ashley/core/ComponentMapper.hpp>

#include "components/Battling.hpp"
#include "components/DeathMarker.hpp"
#include "components/Inventory.hpp"
#include "components/Interactible.hpp"
#include "components/Position.hpp"
#include "components/Renderable.hpp"
#include "components/Named.hpp"

#include "systems/BattleSystem.hpp"

#include "Attack.hpp"
#include "Message.hpp"

void rgl::BattleSystem::handleDeath(ashley::Entity * const entity) const {
	if (entity->hasComponent<Inventory>()) {
		// keep some components temporarily
		entity->toggleComponentOperationHandler();

		auto position = entity->remove<Position>();
		auto inventory = entity->remove<Inventory>();
		auto renderable = entity->remove<Renderable>();
		auto named = entity->remove<Named>();

		renderable->c = '#';

		entity->removeAll();

		entity->add(std::move(position));
		entity->add(std::move(inventory));
		entity->add(std::move(renderable));
		entity->add(std::move(named));
		entity->add<Interactible>(InteractionType::LOOT_CORPSE);

		entity->toggleComponentOperationHandler();
	} else {
		entity->add<DeathMarker>();
	}
}

void rgl::BattleSystem::update(float deltaTime) {
	const auto battlingMapper = ashley::ComponentMapper<Battling>::getMapper();

	for (const auto &attack : attacks) {
		if (attack.from->hasComponent<DeathMarker>() || attack.against->hasComponent<DeathMarker>()) {
			// don't process attacks involving entities that are already dead
			continue;
		}

		const auto fromBat = battlingMapper.get(attack.from);
		const auto againstBat = battlingMapper.get(attack.against);

		const int damage = std::max(fromBat->stats.atk - againstBat->stats.def, 1);
		againstBat->stats.hp -= damage;
		MessageHandler::globalHandler->addAttackMessage(attack, damage);

		if (againstBat->stats.hp > 0) {
			// if still alive, retaliate
			const int retDmg = std::max(againstBat->stats.atk - fromBat->stats.def, 1);
			fromBat->stats.hp -= retDmg;
			MessageHandler::globalHandler->addRetaliationAttackMessage(attack, retDmg);

			if (fromBat->stats.hp <= 0) {
				// handle death of from"
				handleDeath(attack.from);
			}
		} else {
			// handle death of "against"
			handleDeath(attack.against);
		}
	}

	attacks.clear();
}
