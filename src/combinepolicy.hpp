
#ifndef COMBINEIPOLICY_HPP_
#define COMBINEIPOLICY_HPP_

namespace gc {

template<class ParticleType, class Policy1, class Policy2>
class combineinitializerpolicy : public Policy1, public Policy2 {
public:

	typedef ParticleType particle_type;
	typedef Policy1 policy1_type;
	typedef Policy2 policy2_type;

	combineinitializerpolicy();
	combineinitializerpolicy(const policy1_type& p1, const policy2_type& p2);

	void operator()(particle_type& p) const;
};

template<class ParticleType, class Policy1, class Policy2>
class combineactionpolicy : public Policy1, public Policy2 {
public:

	typedef ParticleType particle_type;
	typedef Policy1 policy1_type;
	typedef Policy2 policy2_type;

	combineactionpolicy();
	combineactionpolicy(const policy1_type& p1, const policy2_type& p2);

	bool operator()(particle_type& p, const float frame_rate) const;
};


//Implentation

template<class PT, class P1, class P2>
combineinitializerpolicy<PT, P1, P2>::combineinitializerpolicy() {}

template<class PT, class P1, class P2>
combineinitializerpolicy<PT, P1, P2>::combineinitializerpolicy(const policy1_type& p1, const policy2_type& p2)
	: policy1_type(p1), policy2_type(p2) {}


template<class PT, class P1, class P2>
void combineinitializerpolicy<PT, P1, P2>::operator()(particle_type& p) const {
	policy1_type::operator()(p);
	policy2_type::operator()(p);
}

template<class PT, class P1, class P2>
combineactionpolicy<PT, P1, P2>::combineactionpolicy() {}

template<class PT, class P1, class P2>
combineactionpolicy<PT, P1, P2>::combineactionpolicy(const policy1_type& p1, const policy2_type& p2)
	: policy1_type(p1), policy2_type(p2) {}

template<class PT, class P1, class P2>
bool combineactionpolicy<PT, P1, P2>::operator()(particle_type& p, const float frame_rate) const {
	//&& shortcuts
	return policy1_type::operator()(p, frame_rate) && policy2_type::operator()(p, frame_rate);
}

} //namespace gc

#endif /* COMBINEPOLICY_HPP_ */
