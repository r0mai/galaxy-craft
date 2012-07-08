
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

	virtual void draw(sf::RenderWindow& window) const;

private:
	std::vector<vector2df> path_points;
};

} //namespace gc


#endif /* PATH_HPP_ */
