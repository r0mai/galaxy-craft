

#ifndef UNIT_HPP_
#define UNIT_HPP_

#include "vector2d.hpp"
#include "object.hpp"

namespace gc {

class unit : public object {
public:
	unit();
	unit(const vector2df& position);
private:

};

} //namespace gc

#endif /* UNIT_HPP_ */
