
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "path.hpp"
#include "unit.hpp"
#include "gamemap.hpp"
#include "polygon.hpp"
#include "imanager.hpp"
namespace gc {

class gamemanager {
public:
	gamemanager(unsigned width, unsigned height);

	void run();
	imagemanager manager;

private:
	sf::RenderWindow window;
	vector2di window_size;

	gamemap map;

	std::vector<unit> units;

	sf::View mapview;

	std::string frame_rate_str;

	void init();

	void process_events();
    void advance(const float frame_rate);
    void draw();

	

};

} //namespace gc


#endif /* GAMEMANAGER_HPP_ */
