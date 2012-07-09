
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
	virtual void draw(sf::RenderWindow& window) const;
	virtual ~object();
	
private:
	// collision model stuff:
	vector2df center;
	float radius;

	float orientation; // radians hopefully.
	sf::Sprite sprite; 

protected:
	object(const object& rhs);
	
};


} // namespace gc.

#endif
