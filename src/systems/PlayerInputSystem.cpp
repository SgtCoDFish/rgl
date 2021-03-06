#include <cstdio>

#include <Ashley/core/ComponentMapper.hpp>

#include <glm/vec2.hpp>
#include <libtcod.hpp>

#include "components/Position.hpp"
#include "components/PlayerInputListener.hpp"
#include "components/Inventory.hpp"
#include "components/Interactible.hpp"
#include "components/Battling.hpp"
#include "components/DeathMarker.hpp"

#include "systems/PlayerInputSystem.hpp"

#include "RGL.hpp"
#include "Map.hpp"
#include "Message.hpp"

void rgl::PlayerInputSystem::processEntity(ashley::Entity * const entity, float deltaTime) {
	const auto inputComponent = ashley::ComponentMapper<PlayerInputListener>::getMapper().get(entity);

	switch (mode) {
	case PISMode::REGULAR: {
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
			break;
		}
		break;
	}

	break;
}

case PISMode::IN_MENU: {
	bool closePressed = escPressed;
	const auto topMenu = menuManager->getTopMenu();
	if (topMenu != nullptr) {
		switch (topMenu->type) {
			case MenuType::INVENTORY: {
				if (iPressed) {
					closePressed = true;
					break;
				}
			}

			case MenuType::PLAYER_DETAILS: {
				if (pPressed) {
					closePressed = true;
					break;
				}
			}

			case MenuType::EQUIPMENT: {
				// NYI
				break;
			}

			case MenuType::ITEM_DETAILS: {
				// NYI
				break;
			}

			case MenuType::YES_NO: {
				break;
			}
		}

	} else {
		closePressed = true;
	}

	if (closePressed) {
		if (menuManager->popMenu()) {
			mode = PISMode::REGULAR;
		}
	}
	break;
}
}
}

void rgl::PlayerInputSystem::processNormalState(ashley::Entity * const &entity, float deltaTime,
        PlayerInputListener * const listener) {
	const auto pos = ashley::ComponentMapper<Position>::getMapper().get(entity);
	glm::ivec2 target(pos->position);

	if (iPressed) {
		menuManager->pushInventoryMenu(entity);
		mode = PISMode::IN_MENU;
		return;
	} else if (pPressed) {
		menuManager->pushPlayerMenu(entity);
		mode = PISMode::IN_MENU;
		return;
	}

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
			for (const auto &e : targetTile->contains) {
				const auto interactible = ashley::ComponentMapper<Interactible>::getMapper().get(e);

				if (interactible == nullptr) {
					continue;
				} else {
					listener->choice = e;

					switch (interactible->type) {
					case InteractionType::LOOT_CHEST: {
						menuManager->pushYesNoMenu("There's a chest here. Open it? (Y/N)");
						listener->state = PlayerInputState::RESPONDING;
						return;
					}

					case InteractionType::LOOT_CORPSE: {
						menuManager->pushYesNoMenu("Loot the corpse? (Y/N)");
						listener->state = PlayerInputState::RESPONDING;
						return;
					}

					case InteractionType::FIGHT: {
						battleSystem->addAttack(Attack(entity, e));
						break;
					}

					default: {
						break;
					}
					}
				}
			}
		} else {
			MessageHandler::globalHandler->addMessage("There's nothing there.");
		}
	}

	if (interactionDone) {
		listener->choice = nullptr;
		listener->target.x = -1;
		listener->target.y = -1;
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
				MessageHandler::globalHandler->addMessage("Huh? You could've sworn there was something here!");
			} else {
				// interactible is still there so proceed with interaction.
				const auto interactible = ashley::ComponentMapper<Interactible>::getMapper().get(listener->choice);

				switch (interactible->type) {
				case InteractionType::LOOT_CORPSE:
				case InteractionType::LOOT_CHEST: {
					const auto inventory = ashley::ComponentMapper<Inventory>::getMapper().get(listener->choice);

					if (inventory == nullptr || inventory->contents.empty()) {
						MessageHandler::globalHandler->addMessage("Seems to be empty...");
					} else {
						if (interactible->type == InteractionType::LOOT_CHEST) {
							MessageHandler::globalHandler->addMessage("The chest creaks open.");
						}

						const auto playerInventory = ashley::ComponentMapper<Inventory>::getMapper().get(entity);

						if (playerInventory != nullptr) {
							for (const auto &item : inventory->contents) {
								MessageHandler::globalHandler->addObtainedMessage(item.name);
								playerInventory->contents.push_back(item);
							}

							inventory->contents.clear();
						}
					}

					if (interactible->type == InteractionType::LOOT_CORPSE) {
						listener->choice->add<DeathMarker>();
					}

					break;
				}

//				case InteractionType::FIGHT: {
//					const auto battling = ashley::ComponentMapper<Battling>::getMapper().get(entity);
//
//					if (battling == nullptr) {
//						MessageHandler::globalHandler->addMessage(
//						        "It looks defenceless... you can't bring yourself to attack it.");
//					} else {
//						MessageHandler::globalHandler->addMessage("You bash it with your stick.");
//					}
//
//					break;
//				}

				default: {
					break;
				}
				}
			}
		} else if (nPressed) {
			MessageHandler::globalHandler->addMessage("Never mind.");
		}

		menuManager->popMenu();
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

			case 'i':
			case 'I': {
				iPressed = true;
				break;
			}

			case 'p':
			case 'P': {
				pPressed = true;
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

		case TCODK_ESCAPE: {
			escPressed = true;
			break;
		}

		default:
			break;
		}

		ashley::IteratingSystem::update(deltaTime);

		resetPressedKeys();
	}
}
