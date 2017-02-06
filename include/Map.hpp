#ifndef INCLUDE_MAP_HPP_
#define INCLUDE_MAP_HPP_

#include <memory>
#include <vector>

#include <Ashley/core/Entity.hpp>
#include <Ashley/core/ComponentMapper.hpp>

#include "libtcod.hpp"

#include "easylogging++.h"

#include "components/Position.hpp"

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

	void registerTileContents(ashley::Entity * const &entity) {
		const auto pos = ashley::ComponentMapper<Position>::getMapper().get(entity);

		if (pos != nullptr) {
			const auto tile = getTileAt(pos->position.x, pos->position.y);

			if (tile != nullptr) {
				tile->contains.emplace_back(entity);
			} else {
				RGLL->debug("Can't place entity at (%v, %v) because the tile doesn't exist.", pos->position.x, pos->position.y);
			}
		} else {
			RGLL->debug("Tried to registerTileContents with entity which has no position");
		}
	}

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
