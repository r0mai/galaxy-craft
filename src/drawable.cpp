
#include "drawable.hpp"

namespace gc {

drawable::drawable(const sf::Color& color) : color(color) {}

void drawable::set_color(const sf::Color& col) {
	color = col;
}

const sf::Color& drawable::get_color() const {
	return color;
}

} //namespace gc


