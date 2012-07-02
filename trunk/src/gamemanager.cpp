
#include "gamemanager.hpp"

#include <boost/lexical_cast.hpp>

namespace gc {

gamemanager::gamemanager(unsigned width, unsigned height) :
	window(sf::VideoMode(width, height, 32), "Galaxy Craft")
{

}

void gamemanager::run() {

	while(window.IsOpened()) {
		const float frame_rate = window.GetFrameTime();
		frame_rate_str = boost::lexical_cast<std::string>(1.f/frame_rate);

        process_events();
        advance(frame_rate);
        draw();
	}

}

void gamemanager::process_events() {


}

void gamemanager::advance(const float frame_rate) {


}

void gamemanager::draw() {
	window.Clear();

	window.Draw( sf::String(frame_rate_str) );

	window.Display();
}


} //namespace gc
