#include <string>
#include <utility>

#include <Ashley/Ashley.hpp>

#include "libtcod.hpp"

#include "RGL.hpp"

#define ELPP_STL_LOGGING
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

int main() {
	const auto rgl = std::make_unique<rgl::RGL>("rgl :: woo");

	rgl->init();

	RGLL->debug("rgl initialised.");

	while (!TCODConsole::isWindowClosed()) {
		rgl->update(0.1f);
	}

	return 0;
}
