#ifndef INCLUDE_SYSTEMS_BATTLESYSTEM_HPP_
#define INCLUDE_SYSTEMS_BATTLESYSTEM_HPP_

#include <vector>

#include <Ashley/core/EntitySystem.hpp>

#include "Attack.hpp"

namespace ashley {
class Entity;
}

namespace rgl {

class BattleSystem: public ashley::EntitySystem {
private:
	std::vector<Attack> attacks;

	void handleDeath(ashley::Entity * const entity) const;

public:
	explicit BattleSystem(int priority) :
			EntitySystem(priority) {
	}

	bool checkProcessing() override {
		return !attacks.empty();
	}

	void update(float deltaTime) override;

	void addAttack(const Attack &attack) {
		attacks.push_back(Attack(attack));
	}
};

}

#endif /* INCLUDE_SYSTEMS_BATTLESYSTEM_HPP_ */
