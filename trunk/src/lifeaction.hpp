/*
 * lifeaction.hpp
 *
 *  Created on: Jul 28, 2012
 *      Author: r0mai
 */

#ifndef LIFEACTION_HPP_
#define LIFEACTION_HPP_

template<class ParticleType>
class lifeaction {
public:
	typedef ParticleType particle_type;

	bool operator()(particle_type& p, const float frame_rate) const;

};

//Implementation

template<class PT>
bool lifeaction<PT>::operator()(particle_type& p, const float frame_rate) const {
	p.life_left -= frame_rate;
	return p.life_left > 0.f;
}


#endif /* LIFEACTION_HPP_ */
