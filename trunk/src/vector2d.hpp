

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

#include <cmath>

#include <boost/operators.hpp>

#include <SFML/Graphics.hpp>

namespace gc {

//vector2d can be used for positions as well
template<class T>
class vector2d :
	boost::equality_comparable<vector2d<T>,
	boost::additive<vector2d<T>
	> > {
public:
	vector2d();

	vector2d(const T& x, const T& y);

	//construct from arbitrary vector with x and y components
	//works for sf::Vector2<X> and other vector2d<X>
	template<class U> //Copy constructor and conversion construtor all in one
	vector2d(const U& v);

	sf::Vector2<T> to_sfml_vector() const;

	T length_squared() const;

	//auto length() const -> decltype( std::sqrt( T() ) ) {
	float length() const;

	bool operator==(const vector2d& other) const;

	const vector2d& operator+=(const vector2d& rhs);
	const vector2d& operator-=(const vector2d& rhs);

	void rotate(const T& angle, const vector2d& center);
	vector2d& rotate_copy(const T angle, const vector2d& center) const;

	T x, y;
};

typedef vector2d<int> vector2di;
typedef vector2d<float> vector2df;

//Implementation

template<class T>
vector2d<T>::vector2d() : x(), y() {}

template<class T>
vector2d<T>::vector2d(const T& x, const T& y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

template<class T>
template<class U>
vector2d<T>::vector2d(const U& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

template<class T>
sf::Vector2<T> vector2d<T>::to_sfml_vector() const {
	return sf::Vector2<T>(x, y);
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
bool vector2d<T>::operator==(const vector2d<T>& other) const {
	return x == other.x && y == other.y;
}

template<class T>
const vector2d<T>& vector2d<T>::operator+=(const vector2d<T>& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template<class T>
const vector2d<T>& vector2d<T>::operator-=(const vector2d<T>& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

#include <iostream>

template<class T>
void vector2d<T>::rotate(const T& angle, const vector2d<T>& center) {
	std::cout<<"DEBUG: x:"<<x<<" y:"<<y<<" Angle to rotate: "<<angle<<" center: "<<center.x<<" "<<center.y;
	const T dx = x - center.x;
	const T dy = center.y - y;
	x = center.x + (dx * std::cos(angle)) - (dy * std::sin(angle));
	y = center.y + (dy * std::cos(angle)) - (dx * std::sin(angle));
	std::cout<<" New value: x:"<<x<<" y:"<<y<<std::endl;
}

template<class T>
vector2d<T>& vector2d<T>::rotate_copy(const T angle, const vector2d<T>& center) const{
	vector2d<T> tmp = *this;
	tmp.rotate(angle, center);
	return tmp;
}

} //namespace gc


#endif /* VECTOR2D_HPP_ */
