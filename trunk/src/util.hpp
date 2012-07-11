
#ifndef UTIL_HPP_
#define UTIL_HPP_

namespace gc {

template<class T, class U>
T round_to_int(const U val) {
	//return (val < 0) ? static_cast<T>(val - U(0.5)) : static_cast<T>(val + U(0.5));
	return static_cast<T>(val + U(0.5));
}

} //namespace gc

#endif /* UTIL_HPP_ */
