

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

#include "colored.hpp"
#include "vector2d.hpp"

namespace gc {

//made to work with float (or double) in the first place
template<class T>
class polygon : public sf::Drawable, public colored {
public:
	polygon();

	//Call rerender(), before drawing
	polygon(const std::vector< vector2d<T> >& points);

	//Call rerender(), before drawing
	polygon(const ClipperLib::Polygon& poly);

	VisiLibity::Polygon to_visilibity_polygon() const;
	ClipperLib::Polygon to_clipper_polygon() const;

	//Call rerender(), before drawing
	void add_point(const vector2d<T>& p);

	std::vector< vector2d<T> >& get_points();
	const std::vector< vector2d<T> >& get_points() const;

	const vector2d<T> centroid() const;
	const T area() const;

	void scale(const T ratio);
	void scale(const vector2d<T>& ratio);

	void rotate(const T angle);
//	void rotate(const T angle, const vector2d<T>& center);
	
	void move(const vector2d<T>& direction);

	//this must be called after whenever any function modifying the location of the points is called, for drawing to work properly
	void rerender();

protected:
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const;

private:
	std::vector< vector2d<T> > points;
	sf::ConvexShape sfml_polygon;
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
void polygon<T>::scale(const T ratio) {
	std::for_each( points.begin(), points.end(), [&ratio](vector2d<T>& p) {
		p *= ratio;
	});
}

template<class T>
void polygon<T>::scale(const vector2d<T>& ratio) {
	std::for_each( points.begin(), points.end(), [&ratio](vector2d<T>& p) {
		p *= ratio;
	});
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

template<class T>
void polygon<T>::rerender() {
	sfml_polygon = sf::ConvexShape( points.size() );
	for ( unsigned i = 0; i < points.size(); ++i ) {
		sfml_polygon.setPoint( i, points[i].to_sfml_vector() );
	}
	sfml_polygon.setFillColor( sf::Color::Red );
}

template<class T>
void polygon<T>::draw(sf::RenderTarget& window, sf::RenderStates states) const {
	window.draw(sfml_polygon, states);
}


} //namespace gc


#endif /* POLYGON_HPP_ */
