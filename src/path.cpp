
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

void path::move_forward(float distance) {

	assert(distance >= 0.f);

	if ( is_at_end() ) {
		return;
	}

	float current_segment_length = path_points[position.segment].distance_to( path_points[position.segment + 1] );

	distance -= current_segment_length * ( 1.f - position.advancment_ratio );

	while ( distance >= 0.f && position.segment < path_points.size() - 1 ) {
		++position.segment;
		current_segment_length = path_points[position.segment].distance_to( path_points[position.segment + 1] );
		distance -= current_segment_length;
	}

	if ( position.segment == path_points.size() - 1 ) {
		//We reached the end
		return;
	}

	//distance is negative now
	position.advancment_ratio = (current_segment_length + distance) / current_segment_length;


}

void path::draw(sf::RenderWindow& window) const {
	for ( unsigned i = 0; i < path_points.size() - 1; ++i ) {
		window.Draw( sf::Shape::Line( path_points[i].to_sfml_vector(), path_points[i+1].to_sfml_vector(), 4.f, color ) );
	}
}

path::position_t::position_t() : segment(0), advancment_ratio(0.f) {}

} //namespace gc
