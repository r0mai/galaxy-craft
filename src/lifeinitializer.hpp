

#ifndef LIFEINITIALIZER_HPP_
#define LIFEINITIALIZER_HPP_

#include "util.hpp"

namespace gc {


template<class ParticleType>
class lifeinitializer_exact {
public:

	typedef ParticleType particle_type;

	lifeinitializer_exact(const float life = 1.f);

	void operator()(particle_type& p) const;
private:
	float life;
};

template<class ParticleType>
class lifeinitializer_rangerandom {
public:

	typedef ParticleType particle_type;

	lifeinitializer_rangerandom();
	lifeinitializer_rangerandom(const float min, const float max);

	void operator()(particle_type& p) const;
private:
	float min_life;
	float max_life;
};

//Implementation

template<class PT>
lifeinitializer_exact<PT>::lifeinitializer_exact(const float life) : life(life) {}

template<class PT>
void lifeinitializer_exact<PT>::operator()(particle_type& p) const {
	p.life_left = life;
}

template<class PT>
lifeinitializer_rangerandom<PT>::lifeinitializer_rangerandom() : min_life(0.f), max_life(1.f) {}

template<class PT>
lifeinitializer_rangerandom<PT>::lifeinitializer_rangerandom(const float min, const float max) : min_life(min), max_life(max) {}

template<class PT>
void lifeinitializer_rangerandom<PT>::operator()(particle_type& p) const {
	p.life_left = random_in_range_float( min_life, max_life );
}

} //namespace gc


#endif /* LIFEINITIALIZER_HPP_ */
