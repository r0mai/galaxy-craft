
#include "colored.hpp"

namespace gc {

colored::colored(const sf::Color& c) : color(c) {}

void colored::set_color(const sf::Color& c) {
	color = c;
}

const sf::Color& colored::get_color() const {
	return color;
}

} //namespace gc
