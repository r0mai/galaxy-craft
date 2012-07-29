
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
	mutable sf::Shape cached_rectangle;
};

//Implementation

template<class PT>
drawpolicy<PT>::drawpolicy() :
	cached_rectangle(sf::Shape::Rectangle(sf::Vector2f(-5.f,-5.f), sf::Vector2f(5.f, 5.f), sf::Color::White)) {}

template<class PT>
drawpolicy<PT>::drawpolicy(const vector2df& dimension) :
	cached_rectangle(sf::Shape::Rectangle((dimension/-2.f).to_sfml_vector(), (dimension/2.f).to_sfml_vector(), sf::Color::White)) {}

template<class PT>
void drawpolicy<PT>::operator()(const particle_type& p, sf::RenderWindow& window) const {
	cached_rectangle.SetPosition( p.position.to_sfml_vector() );
	cached_rectangle.SetColor( p.color );
	window.Draw( cached_rectangle );
}

} //namespace gc

#endif /* DRAWPOLICY_HPP_ */
