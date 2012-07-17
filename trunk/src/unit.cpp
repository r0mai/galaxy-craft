

#include "unit.hpp"
#include <boost/math/constants/constants.hpp>

namespace gc {

unit::unit() :
		object(vector2df(0.f, 0.f), 10.f, sf::Image()), state(STANDING), selected(false) {}

unit::unit(const vector2df& position, const float radius, const sf::Image& texture) :
		object(position, radius, texture), state(STANDING), selected(false)  {}

vector2df unit::desired_movement(float distance) {
	moving_path.move_forward( distance );

	return moving_path.get_position() - get_position();
}

void unit::advance(float distance) {
	if ( state == MOVING ) {

		//we advanced a segment, update orientation
		if( moving_path.move_forward( distance ) ){
			set_orientation(moving_path.get_orientation() + boost::math::constants::pi<float>() / 2.f);
		}

		set_position( moving_path.get_position() );

		if ( moving_path.is_at_end() ) {
			state = STANDING;
		}
	}
}

void unit::move_on(const path& p) {
	state = MOVING;
	moving_path = p;

	set_orientation(moving_path.get_orientation() + boost::math::constants::pi<float>() / 2.f);

}

void unit::append_path(const path& p){
	moving_path.append(p);
}

void unit::set_orientation(const float angle){

	// angle is in radians.


	const float deltadegrees = (orientation - angle) * 180 / M_PI;

	sprite.Rotate(deltadegrees);

	orientation = angle;
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
	//If this is called from the outside while the unit is moving on a path,
	//then path readjusting should be done here
}

void unit::draw(sf::RenderWindow& window) const {
	object::draw(window);


	if ( selected ) {
		window.Draw(sf::Shape::Circle(center.to_sfml_vector(), radius, sf::Color(0,0,0,0), 1.f, sf::Color::Green));
	}

	if ( state == MOVING ) {
		moving_path.draw(window);
	}
}

} //namespace gc
