#ifndef INCLUDE_MENUMANAGER_HPP_
#define INCLUDE_MENUMANAGER_HPP_

#include <memory>
#include <vector>

#include "libtcod.hpp"

namespace ashley {
class Entity;
}

namespace rgl {
enum class MenuType {
	INVENTORY, PLAYER_DETAILS, EQUIPMENT, ITEM_DETAILS, YES_NO
};

class Menu {
private:
	std::unique_ptr<TCODConsole> console;

	void renderRect(TCODConsole *console, int x, int y, int w, int h);

	void renderInventory(TCODConsole * target, int targetX, int targetY);
	void renderPlayerDetails(TCODConsole *target, int targetX, int targetY);
	void renderYesNo(TCODConsole *target, int targetX, int targetY);

public:
	const std::string title;
	const MenuType type;

	const int width, height;

	ashley::Entity * const entity;

	explicit Menu(const std::string &title, int w, int h, const MenuType type, ashley::Entity * const entity);

	void render(TCODConsole *target, int x, int y);
};

class MenuManager {
private:
	std::vector<Menu> menus;

public:
	const int maxWidth, maxHeight;

	explicit MenuManager(int maxWidth, int maxHeight);

	void render(TCODConsole *target);

	void pushMenu(Menu &&menu);

	void pushPlayerMenu(ashley::Entity *entity);
	void pushInventoryMenu(ashley::Entity * entity);
	void pushYesNoMenu(const std::string &question);

	// return true if no menus left.
	bool popMenu();

	const Menu * getTopMenu() const;

	inline bool hasActiveMenus() const;
	inline int numActiveMenus() const;
};

}

#endif /* INCLUDE_MENUMANAGER_HPP_ */
