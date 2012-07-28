

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <SFML/Graphics.hpp>
#include "vector2d.hpp"

namespace gc {

struct particle {

	vector2df position;
	vector2df velocity;

	sf::Color color;

	float life_left;
};

} //namespace gc

#endif /* PARTICLE_HPP_ */
