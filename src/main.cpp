#include <string>
#include <utility>

#include <Ashley/Ashley.hpp>

#include "libtcod.hpp"

#include "RGL.hpp"

int main() {
	const auto rgl = std::make_unique<rgl::RGL>("rgl :: woo");

	rgl->init();

	while (!TCODConsole::isWindowClosed()) {
		rgl->update(0.1f);
	}

	return 0;
}
