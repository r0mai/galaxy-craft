
#ifndef PARTICLEGROUP_HPP_
#define PARTICLEGROUP_HPP_

#include <algorithm>
#include <vector>

#include <SFML/Graphics.hpp>

namespace gc {

template<class ParticleType, class InitializePolicy, class ActionPolicy, class DrawPolicy>
class particlegroup {
public:
	typedef ParticleType particle_type;
	typedef InitializePolicy initialize_policy_type;
	typedef ActionPolicy action_policy_type;
	typedef DrawPolicy draw_policy_type;

	particlegroup();
	particlegroup(const initialize_policy_type& initpolicy, const action_policy_type& actionpolicy, const draw_policy_type& drawpolicy);

	void emit(const unsigned amount);

	void advance(const float frame_rate);

	void draw(sf::RenderWindow& window) const;

private:
	initialize_policy_type initialize_policy;
	action_policy_type action_policy;
	draw_policy_type draw_policy;

	std::vector<particle_type> particles;

};

//Implementation

template<class PT, class IP, class AP, class DP>
particlegroup<PT, IP, AP, DP>::particlegroup() {}

template<class PT, class IP, class AP, class DP>
particlegroup<PT, IP, AP, DP>::particlegroup(const initialize_policy_type& initpolicy, const action_policy_type& actionpolicy, const draw_policy_type& drawpolicy) :
	initialize_policy(initpolicy), action_policy(actionpolicy), draw_policy(drawpolicy) {}

template<class PT, class IP, class AP, class DP>
void particlegroup<PT, IP, AP, DP>::emit(const unsigned amount) {
	particles.reserve( particles.size() + amount );
	for ( unsigned i = 0; i < amount; ++i ) {
		particles.push_back( initialize_policy() );
	}
}

template<class PT, class IP, class AP, class DP>
void particlegroup<PT, IP, AP, DP>::advance(const float frame_rate) {
	for ( unsigned i = 0; i < particles.size(); /**/ ) {
		if ( !action_policy( particles[i], frame_rate ) ) {
			//If it's no longer alive, swap to the back, and shrink the vector
			std::swap( particles[i], particles.back() );
			particles.resize( particles.size()-1 );
		} else {
			++i;
		}
	}
}

template<class PT, class IP, class AP, class DP>
void particlegroup<PT, IP, AP, DP>::draw(sf::RenderWindow& window) const {
	for ( unsigned i = 0; i < particles.size(); ++i ) {
		draw_policy(particles[i], window);
	}
}

} //namespace gc

#endif /* PARTICLEGROUP_HPP_ */
