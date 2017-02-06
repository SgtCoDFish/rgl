#ifndef INCLUDE_ENTITYUTIL_HPP_
#define INCLUDE_ENTITYUTIL_HPP_

namespace ashley {
class Engine;
class Entity;
}

namespace rgl {
class Item;

class EntityUtil {
public:
	static ashley::Entity *generateTigerEntity(ashley::Engine &engine, int x, int y);

	static ashley::Entity *generateChestEntity(ashley::Engine &engine, int x, int y, const Item &item);

	static ashley::Entity *generateSabretoothEntity(ashley::Engine &engine, int x, int y);
};

}

#endif /* INCLUDE_ENTITYUTIL_HPP_ */
