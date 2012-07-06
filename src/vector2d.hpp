

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_


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
	vector2d() : x(), y() {}
	vector2d(const T& x, const T& y) : x(x), y(y) {}

	sf::Vector2<T> to_sfml_vector() const {
		return sf::Vector2<T>(x, y);
	}

	bool operator==(const vector2d& other) const {
		return x == other.x && y == other.y;
	}

	const vector2d& operator+=(const vector2d& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	const vector2d& operator-=(const vector2d& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	T x, y;
};

typedef vector2d<float> vector2df;

} //namespace gc


#endif /* VECTOR2D_HPP_ */
