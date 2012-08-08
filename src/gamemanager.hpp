
#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include "SFML/Graphics/RenderTarget.hpp"

#include "path.hpp"
#include "unit.hpp"
#include "gamemap.hpp"
#include "polygon.hpp"
#include "resourcemanager.hpp"
#include "particleinclude.hpp"
#include "logger.hpp"


namespace gc {

class gamemanager {
public:
	gamemanager();

	void run();

private:
	sf::RenderWindow window;
	vector2di window_size;
	logger log;
	resourcemanager manager;

	gamemap map;

	std::vector<unit> units;

	sf::View mapview;

	sf::RenderTexture minimap_texture;


	sf::Font default_font;

	vector2df selection_start;
	bool selection_in_progress;
	bool is_mouse_in_focus;

	std::string frame_rate_str;


	void init();

	void process_events(const float frame_rate);
	void process_keypressed_event(const sf::Event& event);
	void process_mousemoved_event(const sf::Event& event);
	void process_mousebuttonpressed_event(const sf::Event& event);
	void process_mousebuttonreleased_event(const sf::Event& event);
	void process_mousewheelmoved_event(const sf::Event& event);
	void process_textentered_event(const sf::Event& event);


    void advance(const float frame_rate);
    void draw();


	float unit_size;
	float window_mouse_side_rim_ratio;
	float view_move_speed;
	float zoomoutfactor;
	float minimap_scale;
	float unit_engine_particle_density;

};

} //namespace gc


#endif /* GAMEMANAGER_HPP_ */
