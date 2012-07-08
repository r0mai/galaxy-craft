// The idea being, an object has a polygon as its coll model, add some methods,
// Add a sprite/texture on top of it, and we have a fully formed object to play with.

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

namespace gc{

template<class T> // not as relevant now than it was, T is simply the level of precision now.
class object { // Need points as well.
public:

private:
	polygon<T> collision_points;
};

} // namespace gc.

#endif

// Idea being for constuctor: 
// find all non 0 alpha pixels, circumscribe rectangle (should be same as sprite, but have to check anyway)
// circumscribe circle around that, and scale accordingly to radius, centerofcoll = centerofopaque pixels.