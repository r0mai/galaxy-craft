

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <SFML/Graphics.hpp>

#include "vector2d.hpp"
#include "path.hpp"
#include "object.hpp"
#include "particleinclude.hpp"

namespace gc {

class unit : public object {
public:

	enum state_t { STANDING = 0, MOVING };

	unit();
	unit(const vector2df& position, const float radius, const sf::Image& texture);


	vector2df desired_movement(float distance);

	void advance(float frame_rate);
	void advance_movement(float distance);
	void move_on(const path& p);
	void set_orientation(const float angle);
	void append_path(const path& p);

	const vector2df& get_destination() const;

	state_t get_state() const;
	void set_state(const state_t s);

	bool is_selected() const;
	void set_selected(const bool val);
	

	virtual void set_position(const vector2df& pos);

	virtual void draw(sf::RenderWindow& window) const;

private:
	state_t state;

	bool selected;
	sf::Shape selected_circle;

	path moving_path; //used when MOVING



	typedef combineinitializerpolicy<particle, positioninitializer_exact<particle>, velocityinitializer_rectangle<particle>> engine_particlesystem_init_1_type;
	typedef combineinitializerpolicy<particle, colorinitializer_exact<particle>, lifeinitializer_rangerandom<particle>> engine_particlesystem_init_2_type;

	typedef combineinitializerpolicy<particle,
				engine_particlesystem_init_1_type,
				engine_particlesystem_init_2_type
			> engine_particlesystem_init_type;

	typedef combineactionpolicy<particle, lifeaction<particle>, moveaction<particle>> engine_particlesystem_action_type;

	typedef particlegroup<particle,
				engine_particlesystem_init_type,
				engine_particlesystem_action_type,
				drawpolicy<particle>
			> engine_particlesystem_type;

	engine_particlesystem_type engine_particlesystem;

};

} //namespace gc

#endif /* UNIT_HPP_ */
