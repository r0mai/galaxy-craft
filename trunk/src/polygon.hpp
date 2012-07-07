

#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <vector>

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "drawable.hpp"
#include "vector2d.hpp"

namespace gc {

//made to work with float (double) in the first place
template<class T>
class polygon : public drawable {
public:
	polygon();
	polygon(const std::vector< vector2d<T> >& points);

	void add_point(const vector2d<T>& p);

	virtual void draw(sf::RenderWindow& window) const;

	std::vector< vector2d<T> >& get_points();
	const std::vector< vector2d<T> >& get_points() const;

	const vector2d<T> centroid() const;
	const T area() const;

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
void polygon<T>::add_point(const vector2d<T>& p) {
	points.push_back( p );
}

template<class T>
void polygon<T>::draw(sf::RenderWindow& window) const {
	if ( points.size() <= 1 ) {
		return;
	}
	//size 2 will draw a line
	for ( unsigned i = 0; i < points.size() - 1; ++i ) { // Connect all adjacent points.
		window.Draw( sf::Shape::Line( points[i].to_sfml_vector(), points[i+1].to_sfml_vector(), 4.f, color ) );
	}
	// Connect last to first, to complete polygon.
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
	for (unsigned i=0; i < points.size() - 1; ++i){
		area_twice += points[i].x * points[i+1].y - points[i+1].x * points[i].y;
	}
	return area_twice/T(2);

}

template<class T>
const vector2d<T> polygon<T>::centroid() const {
	
	// http://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
	
	T sum_x = T(0);
	T sum_y = T(0);

	for (unsigned i = 0; i <= points.size() - 1; ++i){
		T commonVal = (points[i].x * points[i+1].y - points[i+1].x * points[i].y);
		sum_x += (points[i].x + points[i+1].x) * commonVal;
		sum_y += (points[i].y + points[i+1].y) * commonVal;
	}

	T x, y, area;
	area = area();
	x = sum_x / (T(6) * area );
	y = sum_y / (T(6) * area );
	return vector2d(x,y);

};


} //namespace gc


#endif /* POLYGON_HPP_ */
