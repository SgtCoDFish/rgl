/*
 * RenderSystem.hpp
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

#ifndef INCLUDE_SYSTEMS_RENDERSYSTEM_HPP_
#define INCLUDE_SYSTEMS_RENDERSYSTEM_HPP_

#include <typeinfo>

#include <Ashley/systems/IteratingSystem.hpp>
#include <Ashley/core/Entity.hpp>
#include <Ashley/core/Family.hpp>

#include "components/Renderable.hpp"
#include "components/Position.hpp"

namespace rgl {

class RenderSystem: public ashley::IteratingSystem {
private:
	TCODConsole * const console;

public:
	explicit RenderSystem(TCODConsole * console, int priority = 500000) :
			IteratingSystem(ashley::Family::getFor( { typeid(Renderable), typeid(Position) }), priority), //
			console { console } {
	}

	virtual void processEntity(ashley::Entity * const entity, float deltaTime) override;
};

}

#endif /* INCLUDE_SYSTEMS_RENDERSYSTEM_HPP_ */
