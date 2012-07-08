
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
