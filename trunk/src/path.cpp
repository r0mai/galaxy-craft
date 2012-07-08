
#include "path.hpp"

#include <algorithm>
#include <iterator>

#include "visilibity.hpp"

namespace gc {

path::path() : drawable(sf::Color::Yellow) {}

path::path(const VisiLibity::Polyline& polyline) : drawable(sf::Color::Yellow), path_points( polyline.size() ) {
	for ( unsigned i = 0; i < polyline.size(); ++i ) {
		path_points[i] = polyline[i];
	}
}

path path::search_path(const vector2df& start, const vector2df& end, const vector2df& map_size, const std::vector<polygonf>& obstacles) {

	VisiLibity::Environment enviroment = make_enviroment( map_size, obstacles );

	return path(enviroment.shortest_path( start.to_visilibity_point(), end.to_visilibity_point(), 0.000001 ));

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

VisiLibity::Environment path::make_enviroment( const vector2df& map_size, const std::vector<polygonf>& obstacles ) {

	std::vector<VisiLibity::Point> outer_boundary_vertices; //(0,0), (map_size.x,0), (map_size.x,map_size.y), (0,map_size.y)
	outer_boundary_vertices.push_back( VisiLibity::Point(0, 0) );
	outer_boundary_vertices.push_back( VisiLibity::Point(map_size.x, 0) );
	outer_boundary_vertices.push_back( VisiLibity::Point(map_size.x, map_size.y) );
	outer_boundary_vertices.push_back( VisiLibity::Point(0, map_size.y) );

	//Transform obstacles to a vector which Vislibity::enviroment can accept
	std::vector<VisiLibity::Polygon> vis_enviroment_polygons; //will be <=> obstacles
	vis_enviroment_polygons.reserve( obstacles.size() + 1 );

	//The first polygon in the vector is the outer_boundary
	vis_enviroment_polygons.push_back( VisiLibity::Polygon( outer_boundary_vertices ) );

	std::transform( obstacles.begin(), obstacles.end(), std::back_inserter( vis_enviroment_polygons ),
		[](const polygonf& p){
			return p.to_visilibity_polygon();
		}
	);

	VisiLibity::Environment result(vis_enviroment_polygons);

	result.enforce_standard_form();

	return result;

}

} //namespace gc
