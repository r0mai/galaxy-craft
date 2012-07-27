
#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <SFML/Graphics.hpp>

#include "vector2d.hpp"
#include "drawable.hpp"

namespace gc {

class particle : public drawable {
public:
	particle();

	particle(const vector2df& pos, const vector2df& velocity, const float life, const float radius, const sf::Color& color);

	bool is_alive() const;

	void advance(float frame_rate);

	virtual void draw(sf::RenderWindow& window) const;

private:
	vector2df position;
	vector2df velocity;
	float lifetime;
	float radius;
	//color from drawable
	//TODO maybe draw sprites?
};

} //namespace gc


#endif /* PARTICLE_HPP_ */
