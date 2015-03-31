/*
 * PlayerInputSystem.cpp
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

#include <cstdio>

#include <Ashley/core/ComponentMapper.hpp>

#include <glm/vec2.hpp>
#include <libtcod.hpp>

#include "components/Position.hpp"
#include "components/PlayerInputListener.hpp"
#include "components/Inventory.hpp"
#include "components/Interactible.hpp"

#include "systems/PlayerInputSystem.hpp"

#include "RGL.hpp"
#include "Map.hpp"

#include "easylogging++.h"

void rgl::PlayerInputSystem::processEntity(ashley::Entity * const &entity, float deltaTime) {
	const auto inputComponent = ashley::ComponentMapper<PlayerInputListener>::getMapper().get(entity);

	switch (inputComponent->state) {
	case PlayerInputState::NORMAL: {
		processNormalState(entity, deltaTime, inputComponent);
		break;
	}

	case PlayerInputState::TARGETTING: {
		processTargettingState(entity, deltaTime, inputComponent);
		break;
	}

	default: {
		RGLL->debug("Unhandled case in PlayerInputSystem switch.");
		break;
	}
}
}

void rgl::PlayerInputSystem::processNormalState(ashley::Entity * const &entity, float deltaTime,
        PlayerInputListener * const listener) {
	const auto pos = ashley::ComponentMapper<Position>::getMapper().get(entity);
	glm::ivec2 target(pos->position);

	if (spacePressed) {
		listener->state = PlayerInputState::TARGETTING;
		return;
	} else if (upPressed) {
		--target.y;
	} else if (downPressed) {
		++target.y;
	} else if (leftPressed) {
		--target.x;
	} else if (rightPressed) {
		++target.x;
	}

	// check if target is clear
	if (target.x >= 0 && target.x < map->getWidth() && target.y >= 0 && target.y < map->getHeight()) {
		const auto targetTile = map->getTileAt(target.x, target.y);

		if (targetTile != nullptr && !targetTile->solid) {
			pos->position = target;
		}
	}
}

void rgl::PlayerInputSystem::processTargettingState(ashley::Entity * const &entity, float deltaTime,
        PlayerInputListener * const listener) {
	const auto pos = ashley::ComponentMapper<Position>::getMapper().get(entity);
	glm::ivec2 target(pos->position);
	bool interactionDone = false;

	if (spacePressed) {
		interactionDone = true;
	} else if (upPressed) {
		interactionDone = true;
		--target.y;
	} else if (downPressed) {
		interactionDone = true;
		++target.y;
	} else if (leftPressed) {
		interactionDone = true;
		--target.x;
	} else if (rightPressed) {
		interactionDone = true;
		++target.x;
	}

	const auto targetTile = map->getTileAt(target.x, target.y);

	if (targetTile != nullptr) {
		if (!targetTile->contains.empty()) {
			for (const auto &i : targetTile->contains) {
				const auto inventory = ashley::ComponentMapper<Inventory>::getMapper().get(i);

				for(const auto &item : inventory->contents) {
					RGLL->debug("(%v, %v) contains %v.", target.x, target.y, item.name);
				}
			}
		}
	}

	if(interactionDone) {
		listener->state = PlayerInputState::NORMAL;
	}
}

void rgl::PlayerInputSystem::update(float deltaTime) {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr);

	if (key.vk != TCODK_NONE) {
		if (key.vk == TCODK_CHAR) {
			switch (key.c) {
			case 'w':
			case 'W': {
				key.vk = TCODK_UP;
				break;
			}

			case 's':
			case 'S': {
				key.vk = TCODK_DOWN;
				break;
			}

			case 'a':
			case 'A': {
				key.vk = TCODK_LEFT;
				break;
			}

			case 'd':
			case 'D': {
				key.vk = TCODK_RIGHT;
				break;
			}

			default: {
				break;
			}
			}
		}

		switch (key.vk) {
		case TCODK_UP: {
			upPressed = true;
			break;
		}

		case TCODK_DOWN: {
			downPressed = true;
			break;
		}

		case TCODK_LEFT: {
			leftPressed = true;
			break;
		}

		case TCODK_RIGHT: {
			rightPressed = true;
			break;
		}

		case TCODK_SPACE: {
			spacePressed = true;
			break;
		}

		default:
			break;
		}

		ashley::IteratingSystem::update(deltaTime);

		resetPressedKeys();
	}
}
