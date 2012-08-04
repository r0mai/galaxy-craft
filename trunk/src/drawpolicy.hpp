
#ifndef DRAWPOLICY_HPP_
#define DRAWPOLICY_HPP_

#include <SFML/Graphics.hpp>

#include "vector2d.hpp"

namespace gc {

template<class ParticleType>
class drawpolicy {
public:
	typedef ParticleType particle_type;

	drawpolicy();
	drawpolicy(const vector2df& dimension);

	void operator()(const particle_type& p, sf::RenderWindow& window) const;
private:
	//vector2df dimension;

	//Eww
	mutable sf::RectangleShape cached_rectangle;
};

//Implementation

template<class PT>
drawpolicy<PT>::drawpolicy() :
	cached_rectangle(sf::Vector2f(10.f, 10.f)) {}

template<class PT>
drawpolicy<PT>::drawpolicy(const vector2df& dimension) :
	cached_rectangle(dimension.to_sfml_vector()) {}

template<class PT>
void drawpolicy<PT>::operator()(const particle_type& p, sf::RenderWindow& window) const {
	cached_rectangle.setPosition( p.position.to_sfml_vector() );
	cached_rectangle.setFillColor( p.color );
	window.draw( cached_rectangle );
}

} //namespace gc

#endif /* DRAWPOLICY_HPP_ */
