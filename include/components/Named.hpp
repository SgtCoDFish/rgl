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
