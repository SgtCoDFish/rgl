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

	if (f5Pressed) {
		const auto inventory = ashley::ComponentMapper<Inventory>::getMapper().get(entity);

		if (inventory != nullptr) {
			for (const auto &item : inventory->contents) {
				RGLL->debug("Player has: %v", item.name);
			}
		}
	}

	switch (inputComponent->state) {
	case PlayerInputState::NORMAL: {
		processNormalState(entity, deltaTime, inputComponent);
		break;
	}

	case PlayerInputState::TARGETTING: {
		processTargettingState(entity, deltaTime, inputComponent);
		break;
	}

	case PlayerInputState::RESPONDING: {
		processRespondingState(entity, deltaTime, inputComponent);
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
		listener->target = target;

		if (!targetTile->contains.empty()) {
			listener->choice = targetTile->contains[0];
			RGLL->debug("There's a chest here; open it?");
			listener->state = PlayerInputState::RESPONDING;
			return;
//			for (const auto &i : targetTile->contains) {
//				const auto inventory = ashley::ComponentMapper<Inventory>::getMapper().get(i);
//
//				for (const auto &item : inventory->contents) {
//					RGLL->debug("(%v, %v) contains %v.", target.x, target.y, item.name);
//				}
//			}
		} else {
			RGLL->debug("There's nothing there.");
		}
	}

	if (interactionDone) {
		listener->state = PlayerInputState::NORMAL;
	}
}

void rgl::PlayerInputSystem::processRespondingState(ashley::Entity * const &entity, float deltaTime,
        PlayerInputListener * const listener) {
	if (yPressed || nPressed) {
		if (yPressed) {
			const auto targetTile = map->getTileAt(listener->target.x, listener->target.y);
			bool targetFound = false;

			// make sure the tile still contains the interaction choice.
			if (targetTile != nullptr && listener->choice != nullptr) {
				for (const auto &content : targetTile->contains) {
					if (content == listener->choice) {
						targetFound = true;
						break;
					}
				}
			}

			if (targetTile == nullptr || listener->choice == nullptr || !targetFound) {
				RGLL->debug("Huh? You could've sworn there was something here!");
			} else {
				// interactible is still there so proceed with interaction.
				const auto interactible = ashley::ComponentMapper<Interactible>::getMapper().get(listener->choice);

				switch(interactible->type) {
					case InteractionType::LOOT: {
						const auto inventory = ashley::ComponentMapper<Inventory>::getMapper().get(listener->choice);

						if(inventory == nullptr || inventory->contents.empty()) {
							RGLL->debug("Seems to be empty...");
						} else {
							RGLL->debug("The chest creaks open.");
							const auto playerInventory = ashley::ComponentMapper<Inventory>::getMapper().get(entity);

							if(playerInventory != nullptr) {
								for(const auto &item : inventory->contents) {
									RGLL->debug("You take a %v from the chest.", item.name);
									playerInventory->contents.push_back(item);
								}

								inventory->contents.clear();
							}
						}

						break;
					}

					default: {
						break;
					}
				}
			}
		} else if(nPressed) {
			RGLL->debug("Never mind.");
		}

		listener->state = PlayerInputState::NORMAL;
		listener->target.x = -1;
		listener->target.y = -1;
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

			case 'y':
			case 'Y': {
				yPressed = true;
				break;
			}

			case 'n':
			case 'N': {
				nPressed = true;
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

		case TCODK_F5: {
			f5Pressed = true;
			break;
		}

		default:
			break;
		}

		ashley::IteratingSystem::update(deltaTime);

		resetPressedKeys();
	}
}
