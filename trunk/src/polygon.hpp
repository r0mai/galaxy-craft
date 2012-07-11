

#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>

#include <SFML/Graphics.hpp>

#include "visilibity.hpp"

#include "clipper.hpp"

#include "drawable.hpp"
#include "vector2d.hpp"

namespace gc {

//made to work with float (double) in the first place
template<class T>
class polygon : public drawable {
public:
	polygon();
	polygon(const std::vector< vector2d<T> >& points);

	polygon(const ClipperLib::Polygon& poly);

	VisiLibity::Polygon to_visilibity_polygon() const;
	ClipperLib::Polygon to_clipper_polygon() const;

	void add_point(const vector2d<T>& p);

	virtual void draw(sf::RenderWindow& window) const;

	std::vector< vector2d<T> >& get_points();
	const std::vector< vector2d<T> >& get_points() const;

	const vector2d<T> centroid() const;
	const T area() const;

	void rotate(const T angle);
//	void rotate(const T angle, const vector2d<T>& center);
	
	void move(const vector2d<T>& direction);

private:
	std::vector< vector2d<T> > points;
};

typedef polygon<float> polygonf;

//Implementation :

template<class T>
polygon<T>::polygon() : points() {}

template<class T>
polygon<T>::polygon(const std::vector< vector2d<T> >& points) : points(points) {}

template<class T>
polygon<T>::polygon(const ClipperLib::Polygon& poly) : points(poly.begin(), poly.end()) {}

template<class T>
VisiLibity::Polygon polygon<T>::to_visilibity_polygon() const {
	std::vector<VisiLibity::Point> vispoints;
	vispoints.reserve( points.size() );

	std::transform( points.begin(), points.end(), std::back_inserter( vispoints ),
		[](const vector2d<T>& p) {
			return p.to_visilibity_point();
		}
	);

	return VisiLibity::Polygon(vispoints);

}

template<class T>
ClipperLib::Polygon polygon<T>::to_clipper_polygon() const {
	ClipperLib::Polygon result;
	result.reserve( points.size() );

	std::transform( points.begin(), points.end(), std::back_inserter( result ),
		[](const vector2d<T>& v) {
			return v.to_clipper_point();
		}
	);

	return result;
}

template<class T>
void polygon<T>::add_point(const vector2d<T>& p) {
	points.push_back( p );
}

template<class T>
void polygon<T>::draw(sf::RenderWindow& window) const {
	if ( points.size() <= 1 ) {
		return;
	}

	for ( unsigned i = 0; i < points.size() - 1; ++i ) { // Connect all adjacent points.
		window.Draw( sf::Shape::Line( points[i].to_sfml_vector(), points[i+1].to_sfml_vector(), 4.f, color ) );
	}
	// Connect last to first, to complete the polygon.
	window.Draw( sf::Shape::Line( points.back().to_sfml_vector(), points.front().to_sfml_vector(), 4.f, color ) );
}

template<class T>
std::vector< vector2d<T> >& polygon<T>::get_points() {
	return points;
}

template<class T>
const std::vector< vector2d<T> >& polygon<T>::get_points() const {
	return points;
}

template<class T>
const T polygon<T>::area() const {

	// http://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
	
	T area_twice = T(0);
	for (unsigned i=0; i < points.size() - 1; ++i) {
		area_twice += points[i].x * points[i+1].y - points[i+1].x * points[i].y;
	}

	area_twice = area_twice>T(0) ? area_twice : area_twice*T(-1); // abs.

	return area_twice/T(2);

}

template<class T>
const vector2d<T> polygon<T>::centroid() const {
	// http://www.wolframalpha.com/input/?i=centroid+of+polygon+with+vertices+%28300%2C+200%29%2C+%28200%2C+300%29%2C+%28405%2C+403%29%2C+%28500%2C+320%29
	// based on this, it's safe to say average works best.
	// also checked for 5-gons. Also fixes the glitch we've been having thus far. 
	// Introduces new one though, rotation doesn't seem to do much, apart from shrinking the polygon.
	//
	T x_num = T(0);
	T y_num = T(0);
	T x_den = T(points.size());
	T y_den = T(points.size());
	for(unsigned i=0; i < points.size(); ++i){
		x_num += points[i].x;
		y_num += points[i].y;
	}
	return vector2d<T>(x_num/x_den, y_num/y_den);
}

template<class T>
void polygon<T>::rotate(const T angle){
	// Rotate all points around centroid.

	const vector2d<T> center = centroid();
	std::for_each( points.begin(), points.end(), [&angle, &center](vector2d<T>& p){ p.rotate(angle, center); } );
	

}

template<class T>
void polygon<T>::move(const vector2d<T>& direction){
	// Add direction to all points.

	std::for_each( points.begin(), points.end(), [&direction](vector2d<T>& p){ p += direction; } );
}

} //namespace gc


#endif /* POLYGON_HPP_ */
