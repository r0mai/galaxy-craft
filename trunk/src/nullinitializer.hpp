

#ifndef NULLINITIALIZER_HPP_
#define NULLINITIALIZER_HPP_

namespace gc {

template<class ParticleType>
class nullinitializer {
public:
	typedef ParticleType particle_type;

	void operator()(particle_type& p) const {}
};

} //namespace gc


#endif /* NULLINITIALIZER_HPP_ */
