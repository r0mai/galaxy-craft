
#ifndef DRAWABLE_HPP_
#define DRAWABLE_HPP_

#include <SFML/Graphics.hpp>

namespace gc {

//abstract base class for drawables.
//sf::Drawable is little too complex and doesn't have features we need
class drawable {
public:
	drawable(const sf::Color& color = sf::Color::Black);

	void set_color(const sf::Color& col);
	const sf::Color& get_color() const;

	virtual ~drawable() {}

	virtual void draw(sf::RenderWindow&) const = 0;

protected:

	sf::Color color;

};

} //namespace gc


#endif /* DRAWABLE_HPP_ */
