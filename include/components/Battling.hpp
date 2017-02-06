#ifndef INCLUDE_COMPONENTS_BATTLING_HPP_
#define INCLUDE_COMPONENTS_BATTLING_HPP_

#include <Ashley/core/Component.hpp>

#include "Stats.hpp"

namespace rgl {

class Battling : public ashley::Component {
public:
	Stats stats;

	explicit Battling(const Stats &stats) : stats { stats } {}
};

}

#endif /* INCLUDE_COMPONENTS_BATTLING_HPP_ */
