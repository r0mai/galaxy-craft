
#include "object.hpp"

#include <iostream>

#include <boost/math/constants/constants.hpp>

namespace gc {

object::object() : center(), radius(), orientation(), sprite() {}


object::object(const vector2df& c, const float r, const sf::Texture& texture)
	: center(c), radius(r), orientation(), sprite(texture) {

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
}

void object::move(const vector2df& offset) {
	set_position( center + offset );
}

float object::get_radius() const {
	return radius;
}


void object::draw(sf::RenderTarget& window, sf::RenderStates states) const{
	window.draw(sprite, states);
} // That should be it.

object::~object(){
}

} //namespce gc

