
#include "object.hpp"

namespace gc {

object::object() : center(), radius(), orientation(), sprite() {}


object::object(const std::string& filename, const vector2df& c, const float r) : center(c), radius(r), orientation() {

	// open image by filename, and then assign sprite to it. Not memory effecient!!
	if(!img.LoadFromFile(filename)){
		// Error handler needed soon!
	}
	sprite.SetImage(img);

	// At this point, if image loading was successful at least, image could still be of any size/shape.
	// While this wont result in the tightest collision ring, should be good enough(TM)

	//FIXME this could stay as int for as long as possible, for faster calculation
	float x = static_cast<float>(img.GetWidth());  // static cast because vs doesn't like implicit
	float y = static_cast<float>(img.GetHeight()); // promotion from float to unsigned..

	/*
	   A _a_
		|\  |				__
	  b	| \ |			d = AB = a^2 + b^2
		|__\|
			 B
	*/
	const float diameter = 2.f * radius; // diameter of coll. ring.
	const float d = std::sqrt( x*x + y*y ); // Goal is to ensure d = diameter
	const float lambda = diameter / d;

	sprite.Scale(lambda, lambda);
	sprite.Move( (center - vector2df(radius, radius)).to_sfml_vector()); // Not quite..
	// Work should be done!
}


void object::draw(sf::RenderWindow& window) const{
	window.Draw(sf::Shape::Circle(center.to_sfml_vector(), radius, sf::Color::Green) ); // Need ring instead! argh!
	window.Draw(sprite);
} // That should be it.


} //namespce gc

