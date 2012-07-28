

#ifndef COLORINITIALIZER_HPP_
#define COLORINITIALIZER_HPP_

#include <SFML/Graphics.hpp>

#include "util.hpp"

namespace gc {

template<class ParticleType>
class colorinitializer_exact {
public:

	typedef ParticleType particle_type;

	colorinitializer_exact();
	colorinitializer_exact(
			unsigned char red,
			unsigned char green,
			unsigned char blue);

	void operator()(particle_type& p) const;
private:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

template<class ParticleType>
class colorinitializer_rangerandom {
public:

	typedef ParticleType particle_type;

	colorinitializer_rangerandom();
	colorinitializer_rangerandom(
			unsigned char red_min,
			unsigned char red_max,
			unsigned char green_min,
			unsigned char green_max,
			unsigned char blue_min,
			unsigned char blue_max);

	void operator()(particle_type& p) const;
private:
	unsigned char red_min, red_max;
	unsigned char green_min, green_max;
	unsigned char blue_min, blue_max;
};

//Implementation

template<class PT>
colorinitializer_exact<PT>::colorinitializer_exact() : red(255), blue(255), green(255) {}

template<class PT>
colorinitializer_exact<PT>::colorinitializer_exact(unsigned char red, unsigned char green, unsigned char blue) :
		red(red), green(green), blue(blue) {}


template<class PT>
void colorinitializer_exact<PT>::operator()(particle_type& p) const {
	p.color = sf::Color(red, green, blue);
}

template<class PT>
colorinitializer_rangerandom<PT>::colorinitializer_rangerandom() :
		red_min(0), red_max(255),
		green_min(0), green_max(255),
		blue_min(0), blue_max(255) {}

template<class PT>
colorinitializer_rangerandom<PT>::colorinitializer_rangerandom(
		unsigned char red_min,
		unsigned char red_max,
		unsigned char green_min,
		unsigned char green_max,
		unsigned char blue_min,
		unsigned char blue_max) :
		red_min(red_min), red_max(red_max),
		green_min(green_min), green_max(green_max),
		blue_min(blue_min), blue_max(blue_max) {}

template<class PT>
void colorinitializer_rangerandom<PT>::operator()(particle_type& p) const {
	p.color = sf::Color(
			random_in_range_int( red_min, red_max ),
			random_in_range_int( green_min, green_max ),
			random_in_range_int( blue_min, blue_max ));
}

} //namespace gc


#endif /* COLORINITIALIZER_HPP_ */
