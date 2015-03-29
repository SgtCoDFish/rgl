/*
 * PlayerInputSystem.cpp
 *
 *  Created on: 29 Mar 2015
 *  Author: Ashley Davis (SgtCoDFish)
 */

#include <Ashley/core/ComponentMapper.hpp>

#include <glm/vec2.hpp>
#include <libtcod.hpp>

#include "components/Position.hpp"

#include "systems/PlayerInputSystem.hpp"

#include "RGL.hpp"

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
		pos->position = target;
	}
}

void rgl::PlayerInputSystem::update(float deltaTime) {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr);

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
