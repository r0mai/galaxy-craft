
#include "object.hpp"

#include <iostream>

#include <boost/math/constants/constants.hpp>

namespace gc {

object::object() : center(), radius(), orientation(), sprite(), health(), healthbar() {}


object::object(const vector2df& c, const float r, const sf::Texture& texture)
	: center(c), radius(r), orientation(), sprite(texture), health(), healthbar(sf::Vector2f(r,3.f)) {

	// At this point, if image loading was successful at least, image could still be of any size/shape.
	// While this wont result in the tightest collision ring, should be good enough(TM)

	//FIXME this could stay as int for as long as possible, for faster calculation
	float x = static_cast<float>(texture.getSize().x);  // static cast because VS doesn't like implicit
	float y = static_cast<float>(texture.getSize().y); // promotion from float to unsigned..

	/*
	   A _a_
		|\  |				__
	  b	| \ |			d = AB = a^2 + b^2
		|__\|
			 B
	*/
	const float diameter = 2.f * radius; // diameter of coll. ring.
	const float d = std::sqrt( x*x + y*y ); // Goal is to ensure d = diameter

	const float lambda = ( boost::math::constants::root_two<float>()*diameter / d ); // WTF IS THAT?

	sprite.setOrigin(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height) / 2.f);
	sprite.scale(lambda, lambda);

	
	sprite.setPosition( (center ).to_sfml_vector());

	healthbar.setPosition( (center).to_sfml_vector() + sf::Vector2f(0,radius + 3) );
	healthbar.setOrigin(sf::Vector2f(healthbar.getGlobalBounds().width, healthbar.getGlobalBounds().height) / 2.f);
	healthbar.setFillColor(sf::Color::Green);
	healthbar.setOutlineColor(sf::Color::Green);
	healthbar.setOutlineThickness(1.f);

	// Work should be done!
}

//These two are the same
const vector2df& object::get_center() const {
	return center;
}

const vector2df& object::get_position() const {
	return center;
}

void object::set_position(const vector2df& p) {
	center = p;
	sprite.setPosition( (center).to_sfml_vector());
	healthbar.setPosition( (center).to_sfml_vector() + sf::Vector2f(0, radius + 3) );
}

void object::move(const vector2df& offset) {
	set_position( center + offset );
}

float object::get_radius() const {
	return radius;
}

unsigned object::get_health() const {
	return health;
}

void object::set_health(unsigned h) {
	health = h;
	healthbar.setSize(sf::Vector2f(h/100.f * radius, 3.f));
	
	if(h < 20){
		healthbar.setFillColor(sf::Color::Red);
		healthbar.setOutlineColor(sf::Color::Red);
	}
}
void object::draw(sf::RenderTarget& window, sf::RenderStates states) const{
	window.draw(sprite, states);
	window.draw(healthbar, states);
} // That should be it.

object::~object(){
}

} //namespce gc

