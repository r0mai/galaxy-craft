
#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <cstdlib>

namespace gc {

//for floating point types
template<class T>
T random_in_range_float(const T min, const T max) {
	return (std::rand() / T(RAND_MAX)) * (max-min) + min;
}

//for integer types
//inclusive on both ends
template<class T>
T random_in_range_int(const T min, const T max) {
	return std::rand() % (max - min + 1) + min;
}

template<class T, class U>
T round_to_int(const U val) {
	//return (val < 0) ? static_cast<T>(val - U(0.5)) : static_cast<T>(val + U(0.5));
	return static_cast<T>(val + U(0.5));
}

inline
float get_epsilon() {
	return 0.0001f;
}

} //namespace gc

#endif /* UTIL_HPP_ */
