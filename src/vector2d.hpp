

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

#include <ostream>
#include <cmath>
#include <cstdlib>

#include <boost/operators.hpp>
#include <boost/math/constants/constants.hpp>

#include <SFML/Graphics.hpp>

#include "util.hpp"

#include "clipper.hpp"

#include "visilibity.hpp"

namespace gc {

//vector2d can be used for positions as well
template<class T>
class vector2d :
	boost::equality_comparable<vector2d<T>,
	boost::additive<vector2d<T>,
	boost::multiplicative<vector2d<T>, T,
	boost::multiplicative<vector2d<T>
	> > > > {
public:
	vector2d();

	vector2d(const T& x, const T& y);

	vector2d(const VisiLibity::Point& p);

	vector2d(const ClipperLib::IntPoint& p);

	//construct from arbitrary vector with x and y components
	//works for sf::Vector2<X> and other vector2d<X>
	template<class U> //Copy constructor and conversion construtor all in one
	vector2d(const U& v);

	sf::Vector2<T> to_sfml_vector() const;
	VisiLibity::Point to_visilibity_point() const;
	ClipperLib::IntPoint to_clipper_point() const;

	vector2d normalize() const;

	T length_squared() const;
	float length() const;

	T distance_to_squared(const vector2d<T>& other) const;
	float distance_to(const vector2d<T>& other) const;

	float distance_to_rectangle_squared( vector2d<T> corner1, vector2d<T> corner2 ) const;
	float distance_to_rectangle( const vector2d<T>& corner1, const vector2d<T>& corner2 ) const;

	bool is_in_rectangle( vector2d<T> corner1, vector2d<T> corner2 ) const;

	bool operator==(const vector2d& other) const;

	vector2d& operator-();
	vector2d& operator+();

	vector2d& operator+=(const vector2d& rhs);
	vector2d& operator-=(const vector2d& rhs);

	vector2d& operator*=(const vector2d& rhs);
	vector2d& operator/=(const vector2d& rhs);

	vector2d& operator*=(const T& rhs);
	vector2d& operator/=(const T& rhs);

	//These should only work correctly with floating point T
	void rotate(const T& angle, const vector2d& center);
	vector2d rotate_copy(const T angle, const vector2d& center) const;

	//Unit vector in random direction
	//Should only work correctly with floating point T
	static vector2d random_unit_vector();

	//a vector with length 'length' in a random direction
	static vector2d random_vector(const T& length);

	T x, y;

};


typedef vector2d<int> vector2di;
typedef vector2d<float> vector2df;

inline
vector2d<float> random_vector_in_rectangle(const vector2df& upper_left, const vector2df& lower_right);

//Implementation

template<class T>
vector2d<T>::vector2d() : x(), y() {}

template<class T>
vector2d<T>::vector2d(const T& x, const T& y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

template<class T>
vector2d<T>::vector2d(const VisiLibity::Point& p) : x(static_cast<T>(p.x())), y(static_cast<T>(p.y())) {}

template<class T>
vector2d<T>::vector2d(const ClipperLib::IntPoint& p) : x(p.X), y(p.Y) {}

template<class T>
template<class U>
vector2d<T>::vector2d(const U& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

template<class T>
sf::Vector2<T> vector2d<T>::to_sfml_vector() const {
	return sf::Vector2<T>(x, y);
}

template<class T>
VisiLibity::Point vector2d<T>::to_visilibity_point() const {
	return VisiLibity::Point( x, y );
}

template<class T>
ClipperLib::IntPoint vector2d<T>::to_clipper_point() const {
	return ClipperLib::IntPoint( round_to_int<ClipperLib::long64>(x), round_to_int<ClipperLib::long64>(y) );
}

template<class T>
vector2d<T> vector2d<T>::normalize() const {
	return (*this) / length();
}

template<class T>
T vector2d<T>::length_squared() const {
	return x*x + y*y;
}

template<class T>
float vector2d<T>::length() const {
	return std::sqrt(length_squared());
}

template<class T>
T vector2d<T>::distance_to_squared(const vector2d<T>& other) const {
	return (*this - other).length_squared();
}

template<class T>
float vector2d<T>::distance_to(const vector2d<T>& other) const {
	return std::sqrt(distance_to_squared(other));
}

template<class T>
float vector2d<T>::distance_to_rectangle_squared( vector2d<T> corner1, vector2d<T> corner2 ) const {
	//ensure corner1 is TL, corner2 is BR
    if ( corner1.x > corner2.x ) {
        std::swap(corner1.x, corner2.x);
    }
    if ( corner1.y > corner2.y ) {
        std::swap(corner1.y, corner2.y);
    }
	// determine where we are!
	if (x<corner1.x) { // left
		if (y<corner1.y) { // up
			return distance_to_squared(corner1);
		} else if (y>corner2.y) { // down
			return distance_to_squared(gc::vector2df(corner1.x, corner2.y));
		} else {// middle
			const float dx = corner1.x - x;
			return dx * dx;
		}
	} else 	if (x>corner2.x) { // right
		if (y<corner1.y) { // up
			return distance_to_squared(gc::vector2df(corner2.x, corner1.y));
		} else if (y>corner2.y) { // down
			return distance_to_squared(corner2);
		} else {// middle
			const float dx = x - corner2.x;
			return dx * dx;
		}
	} else { // middle
		if (y<corner1.y) { // up
			const float dy = corner1.y - y;
			return dy * dy;
		} else if (y>corner2.y) { // down
			const float dy = y - corner2.y;
			return dy * dy;
		} else { //middle => in the rectangle
			return 0.f;
		}
	}
}

template<class T>
float vector2d<T>::distance_to_rectangle( const vector2d<T>& corner1, const vector2d<T>& corner2 ) const {
	//TODO this could be a little bit more optimized, since we return values in a*a format from distance_to_rectangle_squared()
	//but if someone wants to call std::sqrt, then he doesn't care about performance anyway
	return std::sqrt( distance_to_rectangle_squared(corner1, corner2) );
}

template<class T>
bool vector2d<T>::is_in_rectangle( vector2d<T> corner1, vector2d<T> corner2 ) const {

    if ( corner1.x > corner2.x ) {
        std::swap(corner1.x, corner2.x);
    }
    if ( corner1.y > corner2.y ) {
        std::swap(corner1.y, corner2.y);
    }

    return corner1.x <= x && corner2.x >= x && corner1.y <= y && corner2.y >= y;
}

template<class T>
bool vector2d<T>::operator==(const vector2d<T>& other) const {
	return x == other.x && y == other.y;
}

template<class T>
vector2d<T>& vector2d<T>::operator-() {
	x = -x;
	y = -y;
	return *this;
}

template<class T>
vector2d<T>& vector2d<T>::operator+() {
	return *this;
}

template<class T>
vector2d<T>& vector2d<T>::operator+=(const vector2d<T>& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template<class T>
vector2d<T>& vector2d<T>::operator-=(const vector2d<T>& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

template<class T>
vector2d<T>& vector2d<T>::operator*=(const vector2d<T>& rhs) {
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}

template<class T>
vector2d<T>& vector2d<T>::operator/=(const vector2d<T>& rhs) {
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}

template<class T>
vector2d<T>& vector2d<T>::operator*=(const T& rhs) {
	x *= rhs;
	y *= rhs;
	return *this;
}

template<class T>
vector2d<T>& vector2d<T>::operator/=(const T& rhs) {
	x /= rhs;
	y /= rhs;
	return *this;
}

template<class T>
void vector2d<T>::rotate(const T& angle, const vector2d<T>& center) {
	// Finally fracking works! 
	// Source: http://content.gpwiki.org/index.php/Rotating_A_Point_In_2D_In_Lua

	T sine = std::sin(angle);
	T cosine = std::cos(angle);
	T distance_x = (x - center.x);
	T distance_y = (y - center.y);

	x = center.x + cosine * distance_x - sine * distance_y;
	y = center.y + sine * distance_x + cosine * distance_y;
}

template<class T>
vector2d<T> vector2d<T>::rotate_copy(const T angle, const vector2d<T>& center) const{
	vector2d<T> tmp = *this;
	tmp.rotate(angle, center);
	return tmp;
}

template<class T>
vector2d<T> vector2d<T>::random_unit_vector() {
	const float theta = (T(std::rand()) / T(RAND_MAX)) * boost::math::constants::two_pi<float>();

	return vector2d<T>( std::cos( theta ), std::sin( theta ) );
}

template<class T>
vector2d<T> vector2d<T>::random_vector(const T& length) {
	return length * random_unit_vector();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const vector2d<T>& v) {
	os << "(" << v.x << "," << v.y << ")";
	return os;
}

vector2df random_vector_in_rectangle(const vector2df& upper_left, const vector2df& lower_right) {
	return vector2df(
			random_in_range_float( upper_left.x, lower_right.x ),
			random_in_range_float( upper_left.y, lower_right.y ) );
}

} //namespace gc


#endif /* VECTOR2D_HPP_ */
