

#include "unit.hpp"

namespace gc {

unit::unit() : object(vector2df(0.f, 0.f), 5.f, sf::Image()), state(STANDING) {}

unit::unit(const vector2df& position, const sf::Image& texture) : object(position, 5.f, texture), state(STANDING) {}

void unit::advance(float distance) {
	if ( state == MOVING ) {
		moving_path.move_forward( distance );
		set_position( moving_path.get_position() );
		if ( moving_path.is_at_end() ) {
			state = STANDING;
		}
	}
}

void unit::move_on(const path& p) {
	state = MOVING;
	moving_path = p;
}

void unit::draw(sf::RenderWindow& window) const {
	object::draw(window);

	if ( state == MOVING ) {
		moving_path.draw(window);
	}
}

} //namespace gc
