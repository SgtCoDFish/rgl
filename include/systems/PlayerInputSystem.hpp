#ifndef __RGL_PLAYER_INPUT_SYSTEM_HPP
#define __RGL_PLAYER_INPUT_SYSTEM_HPP

#include <Ashley/systems/IteratingSystem.hpp>
#include <Ashley/core/Family.hpp>

#include "components/Position.hpp"
#include "components/PlayerInputListener.hpp"

namespace rgl {

class Map;
class BattleSystem;
class MenuManager;

// player input system mode
enum class PISMode {
	REGULAR, IN_MENU
};

class PlayerInputSystem: public ashley::IteratingSystem {
private:
	// remember to update resetPressedKeys too
	bool upPressed = false, downPressed = false, leftPressed = false, rightPressed = false, spacePressed = false;
	bool yPressed = false, nPressed = false, iPressed = false, pPressed = false;
	bool f5Pressed = false, escPressed = false;

	void resetPressedKeys() {
		upPressed = downPressed = leftPressed = rightPressed = spacePressed = yPressed = nPressed = iPressed =
		        pPressed = f5Pressed = escPressed = false;
	}

	inline void processNormalState(ashley::Entity * const &entity, float deltaTime,
	        PlayerInputListener * const listener);
	inline void processTargettingState(ashley::Entity * const &entity, float deltaTime,
	        PlayerInputListener * const listener);
	inline void processRespondingState(ashley::Entity * const &entity, float deltaTime,
	        PlayerInputListener * const listener);

	PISMode mode;

	Map *map = nullptr;
	BattleSystem *battleSystem = nullptr;
	MenuManager *menuManager = nullptr;
public:
	explicit PlayerInputSystem(Map *map, BattleSystem * const battleSystem, MenuManager *menuManager, int priority) :
			IteratingSystem(ashley::Family::getFor( { typeid(Position), typeid(PlayerInputListener) }), priority), //
			mode { PISMode::REGULAR }, //
			map { map }, //
			battleSystem { battleSystem }, //
			menuManager { menuManager } {
	}

	void update(float deltaTime) override;
	virtual void processEntity(ashley::Entity * const entity, float deltaTime) override;

	void setMap(Map *map) {
		this->map = map;
	}

	Map *getMap() const {
		return map;
	}
};

}

#endif //__RGL_PLAYER_INPUT_SYSTEM_HPP
