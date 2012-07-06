
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <string>
#include <SFML/Graphics.hpp>

#include "polygon.hpp"

namespace gc {

class gamemanager {
public:
	gamemanager(unsigned width, unsigned height);

	void run();

private:
	sf::RenderWindow window;
	sf::View view;

	std::vector<polygonf> obstacles;

	std::string frame_rate_str;


	void process_events();
    void advance(const float frame_rate);
    void draw();

};

} //namespace gc


#endif /* GAMEMANAGER_HPP_ */
