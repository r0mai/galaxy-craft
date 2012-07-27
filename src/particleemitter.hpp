

#ifndef PARTICLEEMITTER_HPP_
#define PARTICLEEMITTER_HPP_

#include "drawable.hpp"

namespace gc {

class particleemitter : public drawable {
public:

	virtual ~particleemitter();

	virtual void advance(float frame_rate) = 0;

private:

};

class pointemitter : public particleemitter {
public:

private:

};

} //namespace gc


#endif /* PARTICLEEMITTER_HPP_ */
