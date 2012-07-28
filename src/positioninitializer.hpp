

#ifndef POSITIONINITIALIZER_HPP_
#define POSITIONINITIALIZER_HPP_

#include <algorithm>

#include "util.hpp"
#include "vector2d.hpp"

namespace gc {

template<class ParticleType>
class positioninitializer_exact {
public:

	typedef ParticleType particle_type;

	positioninitializer_exact();
	positioninitializer_exact(const vector2df& position);

	void operator()(particle_type& p) const;
private:
	vector2df position;
};

template<class ParticleType>
class positioninitializer_rectangle {
public:

	typedef ParticleType particle_type;

	positioninitializer_rectangle();
	positioninitializer_rectangle(const vector2df& upper_left, const vector2df& lower_right);

	void operator()(particle_type& p) const;
private:
	vector2df upper_left, lower_right;
};

//Implementation

template<class PT>
positioninitializer_exact<PT>::positioninitializer_exact() : position() {}

template<class PT>
positioninitializer_exact<PT>::positioninitializer_exact(const vector2df& position) : position(position) {}

template<class PT>
void positioninitializer_exact<PT>::operator()(particle_type& p) const {
	p.position = position;
}

template<class PT>
positioninitializer_rectangle<PT>::positioninitializer_rectangle() :
		upper_left(0.f, 0.f), lower_right(100.f, 100.f) {}

template<class PT>
positioninitializer_rectangle<PT>::positioninitializer_rectangle(
		const vector2df& upper_left,
		const vector2df& lower_right) :
		upper_left(upper_left), lower_right(lower_right)
		{
			//swap the x and y values, if not the correct corners are passed
			//(they have to be opposite corners obviously)
			if ( upper_left.x > lower_right.x ) {
				std::swap( upper_left.x, lower_right.x );
			}
			if ( upper_left.y > lower_right.y ) {
				std::swap( upper_left.y, lower_right.y );
			}
		}

template<class PT>
void positioninitializer_rectangle<PT>::operator()(particle_type& p) const {
	p.position = random_vector_in_rectangle( upper_left, lower_right );
}

} //namespace gc



#endif /* POSITIONINITIALIZER_HPP_ */
