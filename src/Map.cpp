#include <utility>
#include <cstdio>
#include <algorithm>
#include <vector>

#include "Map.hpp"

static int ROOM_MIN_SIZE = 6;
static int ROOM_MAX_SIZE = 12;

rgl::Map::Map(int width, int height) :
		tiles(new Tile[width * height]), width(width), height(height) {
	TCODBsp bsp(0, 0, width, height);
	// random, num times, max size, min size, x coeff, y coeff
	bsp.splitRecursive(nullptr, 4, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);

	BSPListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, nullptr);

	std::sort(rooms.begin(), rooms.end(), [](const Room &room1, const Room &room2) {return room1.area > room2.area;});
}

void rgl::Map::dig(int x1, int y1, int x2, int y2) {
	if (x2 < x1) {
		std::swap(x1, x2);
	}

	if (y2 < y1) {
		std::swap(y1, y2);
	}

	for (int y = y1; y <= y2; ++y) {
		for (int x = x1; x <= x2; ++x) {
			const auto tile = getTileAt(x, y);

			if (tile != nullptr) {
				tile->solid = false;
			} else {
				std::printf("Invalid tile to dig at (%d, %d).\n", x, y);
			}
		}
	}
}

bool rgl::BSPListener::visitNode(TCODBsp *node, void *userData) {
	if (node->isLeaf()) {
		int w, h, x, y;

		TCODRandom *rng = TCODRandom::getInstance();

		w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
		h = rng->getInt(ROOM_MAX_SIZE, node->h - 2);
		x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
		y = rng->getInt(node->y + 1, node->y + node->h - h - 1);

		map.dig(x, y, x + w - 1, y + h - 1);
		map.rooms.emplace_back(x, y, x + w - 1, y + h - 1);

		if (roomNumber != 0) {
			// if we're not at the start, dig a corridor.
			map.dig(lastX, lastY, x + w / 2, lastY);
			map.dig(x + w / 2, lastY, x + w / 2, y + h / 2);
		}

		lastX = x + w / 2;
		lastY = y + h / 2;
		++roomNumber;
	}

	return true;
}
