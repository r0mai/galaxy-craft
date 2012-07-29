

#ifndef COLORACTION_HPP_
#define COLORACTION_HPP_

#include <SFML/Graphics.hpp>

namespace gc {

//Interpolates between two colors. The particle will begin with color 'start' and die with color 'end'
template<class ParticleType>
class coloraction {
public:
	typedef ParticleType particle_type;


	coloraction();
	coloraction(const sf::Color& start, const sf::Color& end);

	bool operator()(particle_type& p, const float frame_rate) const;

private:
	sf::Color start, end;
};

//Implementation

template<class PT>
coloraction<PT>::coloraction() : start(sf::Color::White), end(sf::Color::Black) {}

template<class PT>
coloraction<PT>::coloraction(const sf::Color& start, const sf::Color& end) : start(start), end(end) {}


template<class PT>
bool coloraction<PT>::operator()(particle_type& p, const float frame_rate) const {


	if ( p.full_life != 0.f ) {
		const float particle_age = p.full_life - p.life_left;
		p.color = sf::Color(
				start.r + particle_age*( end.r - start.r ) / p.full_life,
				start.g + particle_age*( end.g - start.g ) / p.full_life,
				start.b + particle_age*( end.b - start.b ) / p.full_life
		);
	}

	return true;
}

} //namespace gc


#endif /* COLORACTION_HPP_ */
