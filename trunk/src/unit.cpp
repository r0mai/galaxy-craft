

#include "unit.hpp"

namespace gc {

unit::unit() :
		object(vector2df(0.f, 0.f), 10.f, sf::Image()), state(STANDING), selected(false) {}

unit::unit(const vector2df& position, const sf::Image& texture) :
		object(position, 10.f, texture), state(STANDING), selected(false)  {}

vector2df unit::desired_movement(float distance) {
	moving_path.move_forward( distance );

	return moving_path.get_position() - get_position();
}

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

const vector2df& unit::get_destination() const {
	return moving_path.get_end();
}

unit::state_t unit::get_state() const {
	return state;
}

void unit::set_state(const state_t s) {
	state = s;
}

bool unit::is_selected() const {
	return selected;
}

void unit::set_selected(const bool val) {
	selected = val;
}

void unit::set_position(const vector2df& pos) {
	object::set_position(pos);

	if ( state == MOVING ) {
		moving_path.set_position( pos );
	}
}

void unit::draw(sf::RenderWindow& window) const {
	object::draw(window);

	const float ring_thickness = selected ? 3.f : 1.f;

	window.Draw(sf::Shape::Circle(center.to_sfml_vector(), radius, sf::Color(0,0,0,0), ring_thickness, sf::Color::Green)); // Ring now.

	if ( state == MOVING ) {
		moving_path.draw(window);
	}
}

} //namespace gc
