

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <SFML/Graphics.hpp>

#include <boost/thread.hpp>

#include "vector2d.hpp"
#include "path.hpp"
#include "object.hpp"
#include "particleinclude.hpp"

namespace gc {

class unit : public object {
public:

	enum state_t { STANDING = 0, MOVING };

	unit();
	unit(const vector2df& position, const float radius, const sf::Texture& texture, const float particle_density);


	vector2df desired_movement(float distance);
//	unsigned id;

	void advance(float frame_rate);
	void advance_movement(float distance);

	void move_on_future(const boost::shared_future<path>& path_future);

	void move_on(const path& p);
	void set_orientation(const float angle);
	void append_path(const path& p);

	const vector2df& get_destination() const;

	state_t get_state() const;
	void set_state(const state_t s);

	bool is_selected() const;
	void set_selected(const bool val);
	

	virtual void set_position(const vector2df& pos);

protected:
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const;

private:
	state_t state;

	bool selected;
	sf::CircleShape selected_circle;

	path moving_path; //used when MOVING

	float particle_density;

	int path_calculating;
	boost::shared_future<path> path_future;

	typedef combineinitializerpolicy<particle, positioninitializer_exact<particle>, velocityinitializer_rectangle<particle>> engine_particlesystem_init_1_type;
	typedef combineinitializerpolicy<particle, colorinitializer_exact<particle>, lifeinitializer_rangerandom<particle>> engine_particlesystem_init_2_type;

	typedef combineinitializerpolicy<particle,
				engine_particlesystem_init_1_type,
				engine_particlesystem_init_2_type
			> engine_particlesystem_init_type;

	typedef combineactionpolicy<particle,
				combineactionpolicy<particle,
					coloraction<particle>,
					lifeaction<particle>
				>,
				moveaction<particle>
			> engine_particlesystem_action_type;

	typedef particlegroup<particle,
				engine_particlesystem_init_type,
				engine_particlesystem_action_type,
				drawpolicy<particle>
			> engine_particlesystem_type;

	engine_particlesystem_type engine_particlesystem_fire;
	engine_particlesystem_type engine_particlesystem_smoke;

};

} //namespace gc

#endif /* UNIT_HPP_ */
