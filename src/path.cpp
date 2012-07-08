
#include "path.hpp"

#include <algorithm>
#include <iterator>

#include "visilibity.hpp"

namespace gc {

path::path() : drawable(sf::Color::Yellow) {}

path path::search_path(const vector2df& start, const vector2df& end, const vector2df& map_size, const std::vector<polygonf>& obstacles) {
	path result;
	result.add_point( start );

	//Transform obstacles to a vector which Vislibity::enviroment can accept
	std::vector<VisiLibity::Polygon> vis_enviroment_polygons; //will be <=> obstacles
	vis_enviroment_polygons.reserve( obstacles.size() + 1 );

	std::transform( obstacles.begin(), obstacles.end(), std::back_inserter( vis_enviroment_polygons ),
		[](const polygonf& p){
			return p.to_visilibity_polygon();
		}
	);

	VisiLibity::Environment enviroment(vis_enviroment_polygons);


	result.add_point( end );

	return result;
}

void path::add_point(const vector2df& p) {
	path_points.push_back(p);
}

const vector2df& path::get_start() const {
	return path_points.front();
}

const vector2df& path::get_end() const {
	return path_points.back();
}

void path::draw(sf::RenderWindow& window) const {
	for ( unsigned i = 0; i < path_points.size() - 1; ++i ) {
		window.Draw( sf::Shape::Line( path_points[i].to_sfml_vector(), path_points[i+1].to_sfml_vector(), 4.f, color ) );
	}
}

} //namespace gc
