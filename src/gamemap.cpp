
#include "gamemap.hpp"

#include <sstream>
#include <fstream>

#include "clipper.hpp"

namespace gc {


const double gamemap::vis_epsilon = 0.001;

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

	result.init_visilibity();

	return result;
}

path gamemap::search_path(const vector2df& start, const vector2df& end) const {
	return path(vis_enviroment_offset.shortest_path( start.to_visilibity_point(), end.to_visilibity_point(), vis_visibility_graph_offset, vis_epsilon ));
}

void gamemap::draw(sf::RenderWindow& window) const {
	window.Draw( sf::Shape::Rectangle( sf::Vector2f(0.f,0.f), dimension.to_sfml_vector(), sf::Color(20, 20, 20) ) );

	for ( unsigned i = 0; i < obstacles.size(); ++i ) {
		obstacles[i].draw(window);
	}
	/*for ( unsigned i = 0; i < offset_obstacles.size(); ++i ) {
		offset_obstacles[i].draw(window);
	}*/
}

const vector2df& gamemap::get_dimension() const {
	return dimension;
}

const std::vector<polygonf>& gamemap::get_obstacles() const {
	return obstacles;
}

const VisiLibity::Environment& gamemap::get_vis_enviroment() const {
	return vis_enviroment;
}

void gamemap::init_visilibity() {
	init_enviroment(); //These has to be called in this order
	init_visilibity_graph(); //init_visilibity_graph() relies on init_enviroment()
}

void gamemap::init_enviroment() {

	ClipperLib::Polygons clipper_polygons;
	std::transform( obstacles.begin(), obstacles.end(), std::back_inserter( clipper_polygons ),
		[](const polygonf& p){
			return p.to_clipper_polygon();
		}
	);

	ClipperLib::Polygons result_clipper_polygons;
	ClipperLib::OffsetPolygons( clipper_polygons, result_clipper_polygons, 11., ClipperLib::jtRound, 6. );

	offset_obstacles.reserve(result_clipper_polygons.size());
	std::transform( result_clipper_polygons.begin(), result_clipper_polygons.end(), std::back_inserter( offset_obstacles ),
		[](const ClipperLib::Polygon& p){
			polygonf res(p);
			res.set_color( sf::Color::Yellow );
			return res;
		}
	);

	std::vector<VisiLibity::Point> outer_boundary_vertices; //(0,0), (map_size.x,0), (map_size.x,map_size.y), (0,map_size.y)
	outer_boundary_vertices.push_back( VisiLibity::Point(0, 0) );
	outer_boundary_vertices.push_back( VisiLibity::Point(dimension.x, 0) );
	outer_boundary_vertices.push_back( VisiLibity::Point(dimension.x, dimension.y) );
	outer_boundary_vertices.push_back( VisiLibity::Point(0, dimension.y) );

	//FOR OBSTACLES :
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

	vis_enviroment = VisiLibity::Environment(vis_enviroment_polygons);
	vis_enviroment.enforce_standard_form();

	//FOR OBSTACLES_OFFSET :
	std::vector<VisiLibity::Polygon> vis_enviroment_offset_polygons; //will be <=> obstacles_offset
	vis_enviroment_offset_polygons.reserve( obstacles.size() + 1 );

	//The first polygon in the vector is the outer_boundary
	vis_enviroment_offset_polygons.push_back( VisiLibity::Polygon( outer_boundary_vertices ) );

	std::transform( offset_obstacles.begin(), offset_obstacles.end(), std::back_inserter( vis_enviroment_offset_polygons ),
		[](const polygonf& p){
			return p.to_visilibity_polygon();
		}
	);

	vis_enviroment_offset = VisiLibity::Environment(vis_enviroment_offset_polygons);
	vis_enviroment_offset.enforce_standard_form();

}

void gamemap::init_visilibity_graph() {
	vis_visibility_graph_offset = VisiLibity::Visibility_Graph( vis_enviroment_offset, vis_epsilon );
}

} //namespace gc

