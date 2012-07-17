

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
		bool b = moving_path.move_forward( distance );
		
		vector2df currentpos = get_position();
		vector2df nextpos = moving_path.get_position();

		if( b ){

			const float deltaX = nextpos.x - currentpos.x;
			const float deltaY = nextpos.y - currentpos.y;

			set_orientation(std::atan2f(deltaY, deltaX) + boost::math::constants::pi<float>() / 2.f);
			std::cout<<"Angle calculated" << std::endl;
		}

		set_position( nextpos );	
		

		if ( moving_path.is_at_end() ) {
			set_orientation(0.0f);
			state = STANDING;
		}
	}
}

void unit::move_on(const path& p) {
	state = MOVING;
	moving_path = p;
	// Angle calculation here
		moving_path.move_forward( 0.0001f );
		vector2df currentpos = moving_path.get_start();
		vector2df nextpos = moving_path.get_position();
		const float deltaX = nextpos.x - currentpos.x;
		const float deltaY = nextpos.y - currentpos.y;

		set_orientation(std::atan2f(deltaY, deltaX) + boost::math::constants::pi<float>() / 2.f);
		std::cout<<"Angle calculated" << std::endl;

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
