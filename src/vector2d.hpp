

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
	vector2d(const sf::Vector2<T>&);


	sf::Vector2<T> to_sfml_vector() const;

	T length_squared() const;

	//auto length() const -> decltype( std::sqrt( T() ) ) {
	float length() const;

	bool operator==(const vector2d& other) const;

	const vector2d& operator+=(const vector2d& rhs);
	const vector2d& operator-=(const vector2d& rhs);

	T x, y;
};

typedef vector2d<int> vector2di;
typedef vector2d<float> vector2df;

//Implementation

template<class T>
vector2d<T>::vector2d() : x(), y() {}

template<class T>
vector2d<T>::vector2d(const T& x, const T& y) : x(x), y(y) {}

template<class T>
vector2d<T>::vector2d(const sf::Vector2<T>& v) : x(v.x), y(v.y) {}

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

} //namespace gc


#endif /* VECTOR2D_HPP_ */
