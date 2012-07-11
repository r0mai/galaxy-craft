
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "path.hpp"
#include "unit.hpp"
#include "gamemap.hpp"
#include "polygon.hpp"
#include "resourcemanager.hpp"

namespace gc {

class gamemanager {
public:
	gamemanager(unsigned width, unsigned height);

	void run();

private:
	sf::RenderWindow window;
	vector2di window_size;

	resourcemanager manager;

	gamemap map;

	std::vector<unit> units;

	unit test_unit;
	vector2df test_point;

	sf::View mapview;

	vector2df selection_start;
	bool selection_in_progress;

	std::string frame_rate_str;

	void init();

	void process_events();
	void process_keypressed_event(const sf::Event& event);
	void process_mousemoved_event(const sf::Event& event);
	void process_mousebuttonpressed_event(const sf::Event& event);
	void process_mousebuttonreleased_event(const sf::Event& event);


    void advance(const float frame_rate);
    void draw();

	

};

} //namespace gc


#endif /* GAMEMANAGER_HPP_ */
