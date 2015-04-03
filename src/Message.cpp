/*
 * Message.cpp
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

#include <Ashley/core/ComponentMapper.hpp>

#include "components/Named.hpp"

#include "Attack.hpp"
#include "Message.hpp"

void rgl::MessageHandler::addMessage(const Message &message) {
	messages.push_back(message);
}

void rgl::MessageHandler::addObtainedMessage(const std::string &itemObtained) {
	std::stringstream ss;
	ss << "Obtained a";

	char first = itemObtained.at(0);

	static const std::array<char, 10> vowels = { { 'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U' } };
	if (std::find(vowels.begin(), vowels.end(), first) != vowels.end()) {
		ss << "n";
	}

	ss << " " << itemObtained;

	messages.emplace_back(ss.str());
}

void rgl::MessageHandler::addAttackMessage(const Attack &attack, int damage) {
	std::stringstream ss;

	const auto namedMapper = ashley::ComponentMapper<Named>::getMapper();
	const auto fromNamed = namedMapper.get(attack.from);
	const auto againstNamed = namedMapper.get(attack.against);

	const std::string fName = (fromNamed == nullptr ? "Somebody" : fromNamed->name);
	const std::string aName = (againstNamed == nullptr ? "somebody else" : againstNamed->name);

	ss << fName << " smites " << aName << " for " << damage << " damage!";
	messages.emplace_back(ss.str());
}

void rgl::MessageHandler::addRetaliationAttackMessage(const Attack &attack, int damage) {
	std::stringstream ss;

	// in retaliation, the "against" and "from" swap places.

	const auto namedMapper = ashley::ComponentMapper<Named>::getMapper();
	const auto fromNamed = namedMapper.get(attack.from);
	const auto againstNamed = namedMapper.get(attack.against);

	const std::string fName = (fromNamed == nullptr ? "somebody" : fromNamed->name);
	const std::string aName = (againstNamed == nullptr ? "Somebody else" : againstNamed->name);

	ss << aName << " retaliates against " << fName << " for " << damage << " damage!";
	messages.emplace_back(ss.str());
}

void rgl::MessageHandler::render() const {
	console->setDefaultForeground(textColor);

	const int actualCount = std::min((unsigned int) messages.size(), messageCount);

	for (int i = 0; i < actualCount; ++i) {
		console->print(x, y + i, messages[messages.size() - 1 - i].message.c_str());
	}
}
