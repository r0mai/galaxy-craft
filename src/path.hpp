
#ifndef PATH_HPP_
#define PATH_HPP_

#include <vector>

#include "visilibity.hpp"

#include "polygon.hpp"
#include "vector2d.hpp"
#include "drawable.hpp"

namespace gc {

class path : public drawable {
public:

	path();
	path(const VisiLibity::Polyline& polyline);

	void add_point(const vector2df& p);

	const vector2df& get_start() const;
	const vector2df& get_end() const;

	bool is_at_end() const;
	vector2df get_position() const;

	void set_position(const vector2df& pos);

	//returns the current direction in radians
	float get_orientation() const;

	//Returns true, if we get to a new line segment advancing distance
	//This is useful for orientation calculation
	bool move_forward(float distance);
	void append(const path&);

	virtual void draw(sf::RenderTarget& window) const;

private:
	struct position_t {

		position_t();
		//in [0, path_points.size() - 2] n th segment we're on (surrounded by path_points[segment] and path_points[segment + 1]
		//When == path_points.size() - 1, then we are at the end of the path
		unsigned segment;
		//in [0, 1)
		float advancment_ratio;
	} position;

	std::vector<vector2df> path_points;
};

} //namespace gc


#endif /* PATH_HPP_ */
