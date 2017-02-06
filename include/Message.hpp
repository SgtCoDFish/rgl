#ifndef INCLUDE_COMPONENTS_MESSAGE_HPP_
#define INCLUDE_COMPONENTS_MESSAGE_HPP_

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>

#include "libtcod.hpp"

#include "Attack.hpp"

namespace ashley {
class Entity;
}

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

	void addMessage(const Message &message);

	void addObtainedMessage(const std::string &itemObtained);

	void addAttackMessage(const Attack &attack, int damage);
	void addRetaliationAttackMessage(const Attack &attack, int damage);

	void addDeathMessage(ashley::Entity * const entity);

	void render() const;

	inline void setConsole(TCODConsole * const console) {
		this->console = console;
	}
};

}

#endif /* INCLUDE_COMPONENTS_MESSAGE_HPP_ */
