
#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "visilibity.hpp"

#include "path.hpp"
#include "polygon.hpp"

namespace gc {

class gamemap {
public:
	static gamemap from_file(const std::string& file);

	path search_path(const vector2df& start, const vector2df& end) const;

	const vector2df& get_dimension() const;
	const std::vector<polygonf>& get_obstacles() const;
private:
	vector2df dimension;
	std::vector<polygonf> obstacles;

	VisiLibity::Environment make_enviroment() const;
};

} //namespace gc

#endif /* MAP_HPP_ */
