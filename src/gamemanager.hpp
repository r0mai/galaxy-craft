
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <string>
#include <SFML/Graphics.hpp>

#include "path.hpp"
#include "gamemap.hpp"
#include "polygon.hpp"

namespace gc {

class gamemanager {
public:
	gamemanager(unsigned width, unsigned height);

	void run();

private:
	sf::RenderWindow window;
	vector2di window_size;

	gamemap map;

	sf::View mapview;

	path test_path;

	std::string frame_rate_str;

	void init();

	void process_events();
    void advance(const float frame_rate);
    void draw();

};

} //namespace gc


#endif /* GAMEMANAGER_HPP_ */
