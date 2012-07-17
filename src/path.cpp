
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
	assert( !path_points.empty() );

	return path_points.front();
}

const vector2df& path::get_end() const {

	assert( !path_points.empty() );

	return path_points.back();
}

bool path::is_at_end() const {
	return position.segment == path_points.size() - 1;
}

vector2df path::get_position() const {
	assert( !path_points.empty() );

	//We're at the end
	if ( is_at_end() ) {
		return get_end();
	}

	const vector2df& segment_start = path_points[position.segment];
	const vector2df& segment_end = path_points[position.segment + 1];

	return segment_start + ( segment_end - segment_start )*position.advancment_ratio;

}

void path::set_position(const vector2df& pos) {
	path_points[position.segment] = pos;
	position.advancment_ratio = 0.f;
}

float path::get_orientation() const {

	assert(path_points.size() >= 2);

	vector2df orientation;

	if ( is_at_end() ) {
		orientation = path_points[path_points.size()-1] - path_points[path_points.size()-2];
	} else {
		orientation = path_points[position.segment+1] - path_points[position.segment];
	}

	return std::atan2( orientation.y, orientation.x );
}

bool path::move_forward(float distance) {

	assert(distance >= 0.f);
	assert(path_points.size() >= 2);

	bool b = false;

	if ( is_at_end() ) {
		return false;
	}

	float current_segment_length = path_points[position.segment].distance_to( path_points[position.segment + 1] );

	b = distance >= current_segment_length * ( 1.f - position.advancment_ratio );

	distance -= current_segment_length * ( 1.f - position.advancment_ratio );

	

	while ( distance >= 0.f && position.segment < path_points.size() - 1 ) {
		++position.segment;
		current_segment_length = path_points[position.segment].distance_to( path_points[position.segment + 1] );
		distance -= current_segment_length;
	}


	if ( position.segment == path_points.size() - 1 ) {
		//We reached the end
		return b;
	}

	//distance is negative now
	position.advancment_ratio = (current_segment_length + distance) / current_segment_length;
	return b;


}

void path::draw(sf::RenderWindow& window) const {
	if ( !is_at_end() ) {
		window.Draw( sf::Shape::Line( get_position().to_sfml_vector(), path_points[position.segment+1].to_sfml_vector(), 2.f, color ) );

		for ( unsigned i = position.segment + 1; i < path_points.size() - 1; ++i ) {
			window.Draw( sf::Shape::Line( path_points[i].to_sfml_vector(), path_points[i+1].to_sfml_vector(), 2.f, color ) );
		}
	}
}

void path::append(const path& p){
	path_points.pop_back(); // remove last;
	std::copy(p.path_points.begin(), p.path_points.end(), std::back_inserter(path_points));

}

path::position_t::position_t() : segment(0), advancment_ratio(0.f) {}

} //namespace gc
