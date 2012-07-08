
#include "gamemap.hpp"

#include <sstream>
#include <fstream>

namespace gc {

gamemap gamemap::from_file(const std::string& mapfile) {

	gamemap result;

	std::ifstream in(mapfile.c_str());

	std::string line;

	std::getline( in, line );
	std::stringstream ss(line);
	//TODO syntax checking or xml parsing
	ss >> result.dimension.x >> result.dimension.y;

	while ( std::getline( in, line ) ) {
		std::stringstream ss2(line);

		polygonf poly;
		float x, y;
		while ( ss2 >> x && ss2 >> y ) {
			poly.add_point( vector2df(x, y) );
		}
		poly.set_color( sf::Color::Red );

		if ( poly.get_points().size() >= 3 ) {
			result.obstacles.push_back( poly );
		}

	}

	return result;
}

path gamemap::search_path(const vector2df& start, const vector2df& end) const {
	return path(make_enviroment().shortest_path( start.to_visilibity_point(), end.to_visilibity_point(), 0.000001 ));
}

void gamemap::draw(sf::RenderWindow& window) const {
	window.Draw( sf::Shape::Rectangle( sf::Vector2f(0.f,0.f), dimension.to_sfml_vector(), sf::Color(20, 20, 20) ) );

	for ( unsigned i = 0; i < obstacles.size(); ++i ) {
		obstacles[i].draw(window);
	}
}

const vector2df& gamemap::get_dimension() const {
	return dimension;
}

const std::vector<polygonf>& gamemap::get_obstacles() const {
	return obstacles;
}


VisiLibity::Environment gamemap::make_enviroment() const {

	std::vector<VisiLibity::Point> outer_boundary_vertices; //(0,0), (map_size.x,0), (map_size.x,map_size.y), (0,map_size.y)
	outer_boundary_vertices.push_back( VisiLibity::Point(0, 0) );
	outer_boundary_vertices.push_back( VisiLibity::Point(dimension.x, 0) );
	outer_boundary_vertices.push_back( VisiLibity::Point(dimension.x, dimension.y) );
	outer_boundary_vertices.push_back( VisiLibity::Point(0, dimension.y) );

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

