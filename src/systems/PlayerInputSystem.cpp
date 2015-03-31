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

#include "systems/PlayerInputSystem.hpp"

#include "RGL.hpp"
#include "Map.hpp"

void rgl::PlayerInputSystem::processEntity(ashley::Entity * const &entity, float deltaTime) {
	const auto pos = ashley::ComponentMapper<Position>::getMapper().get(entity);
	glm::ivec2 target(pos->position);

	if (upPressed) {
		target.y--;
	} else if (downPressed) {
		target.y++;
	} else if (leftPressed) {
		target.x--;
	} else if (rightPressed) {
		target.x++;
	}

	// check if target is clear
	if (target.x >= 0 && target.x < RGL::CONSOLE_WIDTH && target.y >= 0 && target.y < RGL::CONSOLE_HEIGHT) {
		const auto targetTile = map->getTileAt(target.x, target.y);

		if (targetTile != nullptr && !targetTile->solid) {
			pos->position = target;
		}
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

		default:
			break;
		}

		ashley::IteratingSystem::update(deltaTime);

		resetPressedKeys();
	}
}
