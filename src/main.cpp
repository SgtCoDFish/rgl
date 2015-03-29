#include <Ashley/AshleyCore.hpp>

#include "libtcod.hpp"

#include "components/Position.hpp"

int main() {
	auto pos = rgl::Position {40, 25};

	TCODConsole::initRoot(80, 50, "rgl", false,
			TCOD_RENDERER_GLSL);

	while (!TCODConsole::isWindowClosed()) {
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
			pos.position.y--;
			break;
		}

		case TCODK_DOWN: {
			pos.position.y++;
			break;
		}

		case TCODK_LEFT: {
			pos.position.x--;
			break;
		}

		case TCODK_RIGHT: {
			pos.position.x++;
			break;
		}

		default:
			break;
		}

		TCODConsole::root->clear();
		TCODConsole::root->putChar(pos.position.x, pos.position.y, '@');
		TCODConsole::flush();
	}
	return 0;
}
