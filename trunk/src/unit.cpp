

#include "unit.hpp"
#include <boost/math/constants/constants.hpp>

namespace gc {

unit::unit() :
		object(vector2df(0.f, 0.f), 10.f, sf::Texture()), state(STANDING), selection_state(UNSELECTED) {}

unit::unit(const vector2df& position, const float radius, const sf::Texture& texture, const float particle_density, const unsigned h) :
		object(position, radius, texture),
		state(STANDING),
		selection_state(UNSELECTED),
		selected_circle(radius),
		particle_density(particle_density),
		path_calculating(false),
		statustext("I"),
		engine_particlesystem_fire(
			engine_particlesystem_init_type(
				engine_particlesystem_init_1_type(
					positioninitializer_exact<particle>(vector2df(0.f, 0.f)),
					velocityinitializer_rectangle<particle>( vector2df(-10.f, -10.f), vector2df(10.f, 10.f) )
				),
				engine_particlesystem_init_2_type(
					colorinitializer_exact<particle>(255, 165, 0),
					lifeinitializer_rangerandom<particle>(.3f, 1.f)
				)
			),
			engine_particlesystem_action_type(
				combineactionpolicy<particle,
					coloraction<particle>,
					lifeaction<particle>>( coloraction<particle>(sf::Color(255, 0, 0), sf::Color(255, 255, 0)), lifeaction<particle>() ),
				moveaction<particle>()
			),
			drawpolicy<particle>(sf::Vector2f(2.f, 2.f))
		),
		engine_particlesystem_smoke(
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
				combineactionpolicy<particle,
					coloraction<particle>,
					lifeaction<particle>>( coloraction<particle>(sf::Color(100, 100, 100), sf::Color(0, 0, 0)), lifeaction<particle>() ),
				moveaction<particle>()
			),
			drawpolicy<particle>(sf::Vector2f(2.f, 2.f))
		)
{
	selected_circle.setOrigin( sf::Vector2f(radius, radius) );
	selected_circle.setFillColor( sf::Color::Transparent );
	selected_circle.setOutlineColor( sf::Color::Green );
	selected_circle.setOutlineThickness( 1.5f );

	//This is little bit redundant, but will make maintenance easier
	set_position( center.to_sfml_vector() );
	health = h;
	statustext.setCharacterSize(10);
	statustext.setColor(sf::Color::Green);
	statustext.setPosition(center.to_sfml_vector() + sf::Vector2f(0, -radius));
	statustext.setOrigin(sf::Vector2f(statustext.getGlobalBounds().width, statustext.getGlobalBounds().height) / 2.f);
	
}

vector2df unit::desired_movement(float distance) {
	moving_path.move_forward( distance );

	return moving_path.get_position() - get_position();
}

void unit::advance(float frame_rate) {

	//TODO handle has_exception()
	if ( path_calculating ) {
		if ( path_future.is_ready() ) {
			std::cout << path_calculating << std::endl;
			path_calculating = false;
			move_on( path_future.get() );
		} else {
			++path_calculating;
		}
	}

	engine_particlesystem_fire.advance(frame_rate);
	engine_particlesystem_smoke.advance(frame_rate);
	engine_particlesystem_fire.emit( particle_density*frame_rate );
	engine_particlesystem_smoke.emit( particle_density*frame_rate );
}

void unit::advance_movement(float distance) {
	if ( state == MOVING ) {

		//we advanced a segment, update orientation
		if( moving_path.move_forward( distance ) ){
			set_orientation(moving_path.get_orientation() + boost::math::constants::pi<float>() / 2.f);
		}

		set_position( moving_path.get_position() );

		if ( moving_path.is_at_end() ) {
			set_state(STANDING);
		}

	}
}

void unit::move_on_future(const boost::shared_future<path>& pf) {
	path_calculating = 1;
	path_future = pf;
}

void unit::move_on(const path& p) {
	// state = MOVING; // Cannot catch state changes if we don't use set_state()!
	set_state(MOVING);
	moving_path = p;

	set_orientation(moving_path.get_orientation() + boost::math::constants::pi<float>() / 2.f);

}

void unit::append_path(const path& p){
	moving_path.append(p);
}

void unit::set_orientation(const float angle){

	// angle is in radians.


	const float deltadegrees = (angle - orientation) * 180 / M_PI;

	sprite.rotate(deltadegrees);

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
	switch (s){
	case unit::IDLE:
		statustext.setString("I");
		break;
	case unit::ATTACKING:
		statustext.setString("A");
		break;
	case unit::DEFENDING:
		statustext.setString("D");
		break;
	case unit::MOVING:
		statustext.setString("M");
		break;
	case unit::PATROLLING:
		statustext.setString("P");
		break;
	case unit::STANDING:
		statustext.setString("S");
		break;
	default:
		statustext.setString("??");
		break;			
	}
}

bool unit::is_selected() const {
	return selection_state == SELECTED;
}

void unit::set_selection_state(const selection_state_t new_state) {
	if ( new_state != selection_state ) {
		selection_state = new_state;

		//Update circle thickness
		switch(selection_state) {
		case SELECTED:
			selected_circle.setOutlineThickness( 1.5f );
			break;
		case POTENTIALLY_SELECTED:
			selected_circle.setOutlineThickness( 0.8f );
			break;
		default:
			break;
		}
	}

}

unit::selection_state_t unit::get_selection_state() const {
	return selection_state;
}

void unit::set_position(const vector2df& pos) {
	object::set_position(pos);

	selected_circle.setPosition( pos.to_sfml_vector() );
	engine_particlesystem_smoke.get_initialize_policy().set_position( pos );
	engine_particlesystem_fire.get_initialize_policy().set_position( pos );

	statustext.setPosition(pos.to_sfml_vector() + sf::Vector2f(0, -radius));

	//If this is called from the outside while the unit is moving on a path,
	//then path readjusting should be done here
}

void unit::draw(sf::RenderTarget& window, sf::RenderStates states) const {
	window.draw(engine_particlesystem_smoke);
	window.draw(engine_particlesystem_fire);

	object::draw(window, states);

	if ( selection_state == SELECTED || selection_state == POTENTIALLY_SELECTED ) {
		window.draw(selected_circle);
	}

	if ( state == MOVING ) {
		window.draw(moving_path);
	}

	window.draw(statustext, states);
}

void unit::set_health(const unsigned h){
	object::set_health(h);
	// death animation here! :D
}


} //namespace gc
