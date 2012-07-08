
#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "vector2d.hpp"
#include <SFML/Graphics.hpp>
#include <string>
namespace gc{

template<class T> // not as relevant now than it was, T is simply the level of precision now.
class object : public drawable { // I want to be able to draw it, duh.
public:
	object();
	object(const std::string& filename, const vector2d<T>& center, const T radius);
	virtual void draw(sf::RenderWindow& window) const;
	
private:
	// collision model stuff:
	vector2d<T> center;
	T radius;

	T orientation; // radians hopefully.
	sf::Sprite sprite; // TODO: cleanup this mess.
	sf::Image img;
	
};

template<class T>
object<T>::object() : center(), radius(T(0)), orientation(T(0)), sprite() {}

template<class T>
object<T>::object(const std::string& filename, const vector2d<T>& c, const T r){
	center = c;
	radius = r;
	orientation = T(0);

	// open image by filename, and then assign sprite to it. Not memory effecient!!
	if(!img.LoadFromFile(filename)){
		// Error handler needed soon!
	}
	sprite.SetImage(img);

	// At this point, if image loading was successful at least, image could still be of any size/shape. 
	// While this wont result in the tightest collision ring, should be good enough(TM)

	T x = static_cast<T>(img.GetWidth());  // static cast because vs doesn't like implicit
	T y = static_cast<T>(img.GetHeight()); // promotion from float to unsigned..

	/* 
	   A _a_
		|\  |				__
	  b	| \ |			d = AB = a^2 + b^2 
		|__\|
			 B
	*/
	T diameter = T(2) * radius; // diameter of coll. ring.
	T d = std::sqrt( x*x + y*y ); // Goal is to ensure d = diameter
	T lambda = diameter / d;	

	sprite.Scale(lambda, lambda);
	sprite.Move(center.to_sfml_vector()); // Not quite..
	// Work should be done!
}


template<class T>
void object<T>::draw(sf::RenderWindow& window) const{
	window.Draw(sf::Shape::Circle(center.to_sfml_vector(), radius, sf::Color::Green) ); // Need ring instead! argh!
	window.Draw(sprite);
} // That should be it.


} // namespace gc.

#endif
