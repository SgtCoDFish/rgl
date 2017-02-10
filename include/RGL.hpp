#ifndef INCLUDE_RGL_HPP_
#define INCLUDE_RGL_HPP_

#include <string>

#include <Ashley/core/Engine.hpp>

#include "systems/MapRenderSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/BattleSystem.hpp"

#include "Map.hpp"
#include "Message.hpp"
#include "MenuManager.hpp"

namespace rgl {

class RGL {
private:
	const std::string windowTitle;

	ashley::Engine engine;

	ashley::Entity *player = nullptr, *mapComponent = nullptr;
	MapRenderSystem *mapRenderSystem = nullptr;
	RenderSystem *renderSystem = nullptr;
	BattleSystem *battleSystem = nullptr;

	Map map;

	MessageHandler messageHandler;

	MenuManager menuManager;

	void renderHUD();

public:
	static const int CONSOLE_WIDTH = 100;
	static const int CONSOLE_HEIGHT = 55;
	static const int STATUS_BAR_HEIGHT = 5;

	explicit RGL(const std::string &windowTitle);

	void init();
	void update(float deltaTime);

	ashley::Entity * getPlayer() const {
		return player;
	}
};

}

#endif
