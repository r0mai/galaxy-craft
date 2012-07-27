
#include "particle.hpp"

namespace gc {

particle::particle() {}

particle::particle(const vector2df& pos, const vector2df& velocity, const float life, const float radius, const sf::Color& color) :
		drawable(color), position(pos), velocity(velocity), lifetime(life), radius(radius) {}

bool particle::is_alive() const {
	return lifetime > 0.f;
}

void particle::advance(float frame_rate) {
	position += frame_rate * velocity;
	lifetime -= frame_rate;
}

void particle::draw(sf::RenderWindow& window) const {
	window.Draw(sf::Shape::Circle( position.to_sfml_vector(), radius, color ));
}

} //namespace gc


