

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



private:
	state_t state;

	path moving_path; //used when MOVING
};

} //namespace gc

#endif /* UNIT_HPP_ */
