
#ifndef COLORED_HPP_
#define COLORED_HPP_

#include <SFML/Graphics.hpp>

namespace gc {

class colored {
public:
	colored(const sf::Color& c = sf::Color::Magenta);

	void set_color(const sf::Color& c);
	const sf::Color& get_color() const;

protected:
	sf::Color color;
};

} //namespace gc

#endif /* COLORED_HPP_ */
