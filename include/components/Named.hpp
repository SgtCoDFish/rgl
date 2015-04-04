/*
 * Named.hpp
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

#ifndef INCLUDE_COMPONENTS_NAMED_HPP_
#define INCLUDE_COMPONENTS_NAMED_HPP_

#include <cctype>

#include <string>

#include <Ashley/core/Component.hpp>

namespace rgl {

// e.g. with name given as "tiger"
// NORMAL -> name = Tiger
// SPECIES  -> name = the tiger
enum class NameType {
	NORMAL, SPECIES
};

class Named: public ashley::Component {
public:
	std::string name;

	Named(const std::string &name, const NameType type = NameType::NORMAL) {
		if (name.length() > 0) {
			switch (type) {
			case NameType::NORMAL: {
				// make sure first letter is capitalised.
				this->name = name;
				this->name[0] = std::toupper(name[0]);

				break;
			}

			case NameType::SPECIES: {
				this->name = "the " + name;
				break;
			}
			}
		}
	}
};

}

#endif /* INCLUDE_COMPONENTS_NAMED_HPP_ */
