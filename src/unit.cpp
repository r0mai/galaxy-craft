

#include "unit.hpp"

namespace gc {

unit::unit() : object(vector2df(0.f, 0.f), 5.f, sf::Image()), state(STANDING) {}

unit::unit(const vector2df& position, const sf::Image& texture) : object(position, 5.f, texture), state(STANDING) {}

} //namespace gc
