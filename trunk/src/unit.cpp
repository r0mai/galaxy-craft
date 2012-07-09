
#include "unit.hpp"

namespace gc {

unit::unit() {

}

unit::unit(const vector2df& position) : object(position, 5.f, "obj.png") {}

} //namespace gc
