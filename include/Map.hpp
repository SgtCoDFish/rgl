/*
 * Map.hpp
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

#ifndef INCLUDE_MAP_HPP_
#define INCLUDE_MAP_HPP_

#include <memory>
#include <vector>

#include <Ashley/core/Entity.hpp>

#include "tcod/libtcod.hpp"

#include "easylogging++.h"

namespace rgl {

struct Tile {
	bool solid = true;

	std::vector<ashley::Entity *> contains;
};

struct Room {
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	int w = 0, h = 0;
	int area = 0;

	Room(int x1, int y1, int x2, int y2) :
			x1(x1), y1(y1), x2(x2), y2(y2) {
		w = x2 - x1;
		h = y2 - y1;

		area = w * h;
	}

	inline bool operator==(const Room &other) const {
		return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
	}
};

class BSPListener;

class Map {
protected:
	std::unique_ptr<Tile[]> tiles;
	std::vector<Room> rooms;
	int width, height;

	void dig(int x1, int y1, int x2, int y2);
	friend class BSPListener;

public:
	explicit Map(int width, int height);

	inline Tile *getTileAt(int x, int y) const {
		if (x < 0 || x > width || y < 0 || y > height) {
			RGLL->debug("Invalid call to getTileAt at: (%v, %v)", x, y);
			return nullptr;
		}

		return &(tiles[width * y + x]);
	}

	inline int getWidth() const {
		return width;
	}

	inline int getHeight() const {
		return height;
	}

	inline Room &getRoom(int i) {
		return rooms[i];
	}

	inline Room &getRandomRoom() {
		return rooms[TCODRandom::getInstance()->getInt(0, rooms.size()-1)];
	}
};

class BSPListener: public ITCODBspCallback {
private:
	Map &map;
	int roomNumber = 0;
	int lastX = -1, lastY = -1;

public:
	explicit BSPListener(Map &map) :
			map(map) {
	}

	bool visitNode(TCODBsp *node, void *userData) override;
};

}

#endif /* INCLUDE_MAP_HPP_ */
