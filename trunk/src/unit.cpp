

#include "unit.hpp"
#include <boost/math/constants/constants.hpp>

namespace gc {

unit::unit() :
		object(vector2df(0.f, 0.f), 10.f, sf::Image()), state(STANDING), selected(false) {}

unit::unit(const vector2df& position, const float radius, const sf::Image& texture) :
		object(position, radius, texture),
		state(STANDING),
		selected(false),
		selected_circle(sf::Shape::Circle(sf::Vector2f(), radius, sf::Color(0,0,0,0), 1.f, sf::Color::Green)),
		engine_particlesystem(
			engine_particlesystem_init_type(
				engine_particlesystem_init_1_type(
					positioninitializer_exact<particle>(vector2df(0.f, 0.f)),
					velocityinitializer_rectangle<particle>( vector2df(-20.f, -20.f), vector2df(20.f, 20.f) )
				),
				engine_particlesystem_init_2_type(
					colorinitializer_exact<particle>(255, 165, 0),
					lifeinitializer_rangerandom<particle>(0.5f, 2.f)
				)
			),
			engine_particlesystem_action_type(
				lifeaction<particle>(),
				moveaction<particle>()
			),
			drawpolicy<particle>(sf::Vector2f(2.f, 2.f))
		)
{
	//This is little bit redundant, but will make maintenance easier
	set_position( center.to_sfml_vector() );
}

vector2df unit::desired_movement(float distance) {
	moving_path.move_forward( distance );

	return moving_path.get_position() - get_position();
}

void unit::advance(float frame_rate) {
	engine_particlesystem.advance(frame_rate);
	engine_particlesystem.emit( 1 );
}

void unit::advance_movement(float distance) {
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

	selected_circle.SetPosition( pos.to_sfml_vector() );
	engine_particlesystem.get_initialize_policy().set_position( pos );

	//If this is called from the outside while the unit is moving on a path,
	//then path readjusting should be done here
}

void unit::draw(sf::RenderWindow& window) const {

	engine_particlesystem.draw(window);
	object::draw(window);

	if ( selected ) {
		sf::Shape circle = sf::Shape::Circle(center.to_sfml_vector(), radius, sf::Color(0,0,0,0), 1.f, sf::Color::Green);

		window.Draw(selected_circle);
	}

	if ( state == MOVING ) {
		moving_path.draw(window);
	}
}

} //namespace gc
