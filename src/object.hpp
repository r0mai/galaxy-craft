
#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "vector2d.hpp"
#include "drawable.hpp"

#include <SFML/Graphics.hpp>
#include <string>

namespace gc {

class object : public drawable { // I want to be able to draw it, duh.
public:
	object();
	object(const vector2df& c, const float r, const sf::Image& img);

	//These two are the same.

	const vector2df& get_center() const;
	const vector2df& get_position() const;

	virtual void set_position(const vector2df& p);

	//Uses set_position internally, so inherited set_positions will work
	void move(const vector2df& offset);

	float get_radius() const;

	virtual void draw(sf::RenderWindow& window) const;
	virtual ~object();
	
protected:
	// collision model stuff:
	vector2df center;
	float radius;

	float orientation; // radians hopefully.
	sf::Sprite sprite; 

};


} // namespace gc.

#endif
