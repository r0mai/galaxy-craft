

#ifndef VELOCITYINITIALIZER_HPP_
#define VELOCITYINITIALIZER_HPP_

namespace gc {

template<class ParticleType>
class velocityinitializer_exact {
public:

	typedef ParticleType particle_type;

	velocityinitializer_exact();
	velocityinitializer_exact(const vector2df& velocity);

	void operator()(particle_type& p) const;
private:
	vector2df velocity;
};

//This is fast, and visually correct for some cases (OK!.. I was lazy...)
template<class ParticleType>
class velocityinitializer_rectangle {
public:

	typedef ParticleType particle_type;

	velocityinitializer_rectangle();
	velocityinitializer_rectangle(const vector2df& upper_left, const vector2df& lower_right);

	void operator()(particle_type& p) const;
private:
	vector2df upper_left, lower_right;
};

//Implementation

template<class PT>
velocityinitializer_exact<PT>::velocityinitializer_exact() : velocity() {}

template<class PT>
velocityinitializer_exact<PT>::velocityinitializer_exact(const vector2df& velocity) : velocity(velocity) {}

template<class PT>
void velocityinitializer_exact<PT>::operator()(particle_type& p) const {
	p.velocity = velocity;
}

template<class PT>
velocityinitializer_rectangle<PT>::velocityinitializer_rectangle() :
		upper_left(-400.f, -200.f), lower_right(-300.f, 200.f) {}

template<class PT>
velocityinitializer_rectangle<PT>::velocityinitializer_rectangle(
		const vector2df& up_left,
		const vector2df& low_right) :
		upper_left(up_left), lower_right(low_right)
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
void velocityinitializer_rectangle<PT>::operator()(particle_type& p) const {
	p.velocity = random_vector_in_rectangle( upper_left, lower_right );
}

} //namespace gc


#endif /* VELOCITYINITIALIZER_HPP_ */
