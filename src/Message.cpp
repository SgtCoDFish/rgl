#include <cctype>

#include <string>
#include <sstream>
#include <array>

#include <Ashley/core/ComponentMapper.hpp>
#include <Ashley/core/Entity.hpp>

#include "components/Named.hpp"

#include "Attack.hpp"
#include "Message.hpp"

void rgl::MessageHandler::addMessage(const Message &message) {
	std::string capMsg = message.message;
	capMsg[0] = std::toupper(capMsg[0]);
	messages.emplace_back(Message(message.priority, capMsg));
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

	addMessage(ss.str());
}

void rgl::MessageHandler::addAttackMessage(const Attack &attack, int damage) {
	std::stringstream ss;

	const auto namedMapper = ashley::ComponentMapper<Named>::getMapper();
	const auto fromNamed = namedMapper.get(attack.from);
	const auto againstNamed = namedMapper.get(attack.against);

	const std::string fName = (fromNamed == nullptr ? "Somebody" : fromNamed->name);
	const std::string aName = (againstNamed == nullptr ? "somebody else" : againstNamed->name);

	ss << fName << " smites " << aName << " for " << damage << " damage!";

	addMessage(ss.str());
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

	addMessage(ss.str());
}

void rgl::MessageHandler::addDeathMessage(ashley::Entity * const entity) {
	const auto named = ashley::ComponentMapper<Named>::getMapper().get(entity);

	const std::string name = (named == nullptr ? "A body" : named->name);

	std::stringstream ss;
	ss << name << "'s lifeless form fades into the aether.";

	addMessage(ss.str());
}

void rgl::MessageHandler::render() const {
	console->setDefaultForeground(textColor);

	const int actualCount = std::min((unsigned int) messages.size(), messageCount);

	for (int i = 0; i < actualCount; ++i) {
		console->print(x, y + i, messages[messages.size() - 1 - i].message.c_str());
	}
}
