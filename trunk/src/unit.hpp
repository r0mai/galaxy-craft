

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <SFML/Graphics.hpp>

#include "vector2d.hpp"
#include "path.hpp"
#include "object.hpp"

namespace gc {

class unit : public object {
public:

	enum state_t { STANDING = 0, MOVING };

	unit();
	unit(const vector2df& position, const sf::Image& texture);


	vector2df desired_movement(float distance);
	void advance(float distance);
	void move_on(const path& p);

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

	path moving_path; //used when MOVING
};

} //namespace gc

#endif /* UNIT_HPP_ */
