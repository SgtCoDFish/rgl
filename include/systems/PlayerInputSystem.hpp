/*
 * PlayerInputSystem.hpp
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

#ifndef __RGL_PLAYER_INPUT_SYSTEM_HPP
#define __RGL_PLAYER_INPUT_SYSTEM_HPP

#include <Ashley/systems/IteratingSystem.hpp>
#include <Ashley/core/Family.hpp>

#include "components/Position.hpp"
#include "components/PlayerInputListener.hpp"

namespace rgl {

class Map;

class PlayerInputSystem: public ashley::IteratingSystem {
private:
	bool upPressed = false, downPressed = false, leftPressed = false, rightPressed = false;

	void resetPressedKeys() {
		upPressed = downPressed = leftPressed = rightPressed = false;
	}

	Map *map = nullptr;
public:
	explicit PlayerInputSystem(Map *map) :
			IteratingSystem(ashley::Family::getFor( { typeid(Position), typeid(PlayerInputListener) }), 1000u), map(map) {
	}

	void update(float deltaTime) override;
	virtual void processEntity(ashley::Entity * const &entity, float deltaTime) override;

	void setMap(Map *map) {
		this->map = map;
	}

	Map *getMap() const {
		return map;
	}
};

}

#endif //__RGL_PLAYER_INPUT_SYSTEM_HPP
