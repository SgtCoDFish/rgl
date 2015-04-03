/*
 * Message.hpp
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

#ifndef INCLUDE_COMPONENTS_MESSAGE_HPP_
#define INCLUDE_COMPONENTS_MESSAGE_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>

namespace rgl {

class Message {
public:
	const unsigned int priority;
	const std::string message;

	Message(const char *message) :
			Message { 0, message } {
	}

	Message(const std::string &message) :
			Message { 0, message } {
	}

	Message(unsigned int priority, const std::string &message) :
			priority { priority }, message { message } {
	}

	Message(unsigned int priority, const char *message) :
			priority { priority }, message { message } {
	}
};

class MessageHandler {
private:
	std::vector<Message> messages;

	TCODConsole * console = nullptr;

	int x = 0, y = 0;

	unsigned int messageCount = 3;

	TCODColor textColor;

public:
	// defined in RGL.cpp
	static MessageHandler * globalHandler;

	explicit MessageHandler(TCODConsole * const console, int x, int y, unsigned int messageCount = 3,
	        const TCODColor &textColor = TCODColor::white) :
			console { console }, x { x }, y { y }, messageCount { messageCount }, textColor { textColor } {
	}

	void addMessage(const Message &message) {
		messages.push_back(message);
	}

	void addObtainedMessage(const std::string &itemObtained) {
		std::stringstream ss;
		ss << "Obtained a";

		char first = itemObtained.at(0);

		static const std::array<char, 10> vowels = {{'a','A','e','E','i','I','o','O','u','U'}};
		if (std::find(vowels.begin(), vowels.end(), first) != vowels.end()) {
			ss << "n";
		}

		ss << " " << itemObtained;

		messages.push_back(ss.str());
	}

	void render() const {
		console->setDefaultForeground(textColor);

		const int actualCount = std::min((unsigned int) messages.size(), messageCount);

		for (int i = 0; i < actualCount; ++i) {
			console->print(x, y + i, messages[messages.size() - 1 - i].message.c_str());
		}
	}

	void setConsole(TCODConsole * const console) {
		this->console = console;
	}
};

}

#endif /* INCLUDE_COMPONENTS_MESSAGE_HPP_ */