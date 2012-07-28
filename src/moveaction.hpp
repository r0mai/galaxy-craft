

#ifndef MOVEACTION_HPP_
#define MOVEACTION_HPP_

namespace gc {

template<class ParticleType>
class moveaction {
public:
	typedef ParticleType particle_type;

	bool operator()(particle_type& p, const float frame_rate) const;

};

//Implementation

template<class PT>
bool moveaction<PT>::operator()(particle_type& p, const float frame_rate) const {

	//std::cout << "pos : " << p.position << " vel : " << p.velocity << std::endl;

	p.position += p.velocity * frame_rate;
	return true;
}

} //namespace gc


#endif /* MOVEACTION_HPP_ */
