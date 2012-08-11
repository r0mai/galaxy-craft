
#include "gamemanager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <memory>

#include <SFML/Audio.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "vector2d.hpp"
#include "object.hpp"
#include "configreader.hpp"

namespace gc {

gamemanager::gamemanager() {
	init();
	log = logger(logger::error);
}



void gamemanager::init() {
	configreader config;

	//Add default values
	config.add_value("screen_width", "800");
	config.add_value("screen_height", "600");
	config.add_value("window_title", "Galaxy Craft");
	config.add_value("map_file", "test.map");
	config.add_value("unit_size", "15");
	config.add_value("obstacle_offset_ratio", "0.8");
	config.add_value("window_mouse_side_rim_ratio", "0.08");
	config.add_value("view_move_speed", "400");
	config.add_value("debug_level", "0");
	config.add_value("zoomoutfactor", "1.05");
	config.add_value("minimap_width", "160");
	config.add_value("minimap_height", "120");
	config.add_value("unit_engine_particle_density", "20");
	config.add_value("font_file", "Sanchezregular.otf");

	//This will overwrite default values
	config.read_config( "gc.cfg" );

	const unsigned window_width = config.get_value<unsigned>( "screen_width" );
	const unsigned window_height = config.get_value<unsigned>( "screen_height" );
	const std::string window_title = config.get_value<std::string>( "window_title" );
	const std::string map_file = config.get_value<std::string>( "map_file" );
	unit_size = config.get_value<float>( "unit_size" );
	const float obstacle_offset_ratio = config.get_value<float>( "obstacle_offset_ratio" );
	window_mouse_side_rim_ratio = config.get_value<float>( "window_mouse_side_rim_ratio" );
	view_move_speed = config.get_value<float>( "view_move_speed" );
	zoomoutfactor = config.get_value<float>( "zoomoutfactor" );
	unit_engine_particle_density = config.get_value<float>( "unit_engine_particle_density" );
	const std::string font_file = config.get_value<std::string>("font_file");
	const unsigned minimap_width = config.get_value<unsigned>("minimap_width");
	const unsigned minimap_height = config.get_value<unsigned>("minimap_height");

	window.create( sf::VideoMode(window_width, window_height, 32), window_title );
	window_size = vector2di(window_width, window_height);

	map = gamemap::from_file( map_file, unit_size * obstacle_offset_ratio );

	mapview = sf::View(sf::Vector2f((map.get_dimension()/2.f).to_sfml_vector()), sf::Vector2f((window_size/2).to_sfml_vector()));

	if ( !default_font.loadFromFile(font_file) ) {
		std::cout << "Couldn't load font file \"" << font_file << "\"" << std::endl;
	}

	selection_in_progress = false;
	is_mouse_in_focus = true;
	frame_rate_str = "unknown";

	game_minimap = minimap( map.get_dimension(), vector2df(20, 50), vector2di(minimap_width, minimap_height) );
	game_minimap.render_background( map.get_obstacles() );
}

void gamemanager::run() {

	sf::Clock frame_clock;

	while(window.isOpen()) {
		const float frame_rate = frame_clock.restart().asSeconds();
		frame_rate_str = boost::lexical_cast<std::string>( static_cast<int>(1.f/frame_rate) );

        process_events(frame_rate);

        advance(frame_rate);
        draw();
	}

}

void gamemanager::process_events(const float frame_rate) {
	sf::Event event;
	while( window.pollEvent( event ) ) {
		switch ( event.type ) {
		case sf::Event::Closed :
			window.close();
			break;
		case sf::Event::KeyPressed :
			process_keypressed_event(event);
			break;
		case sf::Event::MouseMoved :
			process_mousemoved_event(event);
			break;
		case sf::Event::MouseButtonPressed :
			process_mousebuttonpressed_event(event);
			break;
		case sf::Event::MouseButtonReleased :
			process_mousebuttonreleased_event(event);
			break;
		case sf::Event::MouseWheelMoved :
			process_mousewheelmoved_event(event);
			break;
		case sf::Event::MouseEntered :
			is_mouse_in_focus = true;
			break;
		case sf::Event::MouseLeft :
			is_mouse_in_focus = false;
			break;
		case sf::Event::TextEntered :
			gamemanager::process_textentered_event(event);
			break;
		default:
			break;
		}
	}



	float Offset = view_move_speed * frame_rate;

	//TODO maybe the rim size should be independent of the direction (up-down, or left-right)
	const float rim = window_mouse_side_rim_ratio;
	const float upthreshold = window.getSize().y   * rim;
	const float downthreshold = window.getSize().y * (1-rim);
	const float leftthreshold = window.getSize().x  * rim;
	const float rightthreshold = window.getSize().x * (1-rim);

	
    // Move the view using arrow keys or by mouse position
	// TODO: add check for out of rangness, and is window in focus

	const sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up )   || (mouse_position.y < upthreshold    && is_mouse_in_focus  ) ) {
		mapview.move( 0, -Offset);
	}
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down )  || (mouse_position.y > downthreshold  && is_mouse_in_focus  ) ) {
		mapview.move( 0, Offset);
    }
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left )  || (mouse_position.x < leftthreshold  && is_mouse_in_focus  ) ) {
		mapview.move(-Offset, 0);
	}
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) || (mouse_position.x > rightthreshold && is_mouse_in_focus  ) ) {
		mapview.move( Offset, 0);
	}

	

}

void gamemanager::process_keypressed_event(const sf::Event& event) {
	switch ( event.key.code ) {
	case sf::Keyboard::Escape :
		window.close();
		break;
	case sf::Keyboard::Space :
		{

			const vector2df destination = vector2df(window.convertCoords( sf::Mouse::getPosition(window), mapview ));
			if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {
				if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {
					units.push_back( unit( destination, unit_size, manager.get_texture("obj.png"), unit_engine_particle_density ) );
				}
			}
		}
		break;
	case sf::Keyboard::R:
		std::for_each(units.begin(), units.end(), [](unit& u){u.set_orientation(M_PI/4);} );
		break;
	case sf::Keyboard::A:
		if ( event.key.control ) {// CTRL A
			std::for_each(units.begin(), units.end(),
				[](unit& u) {
					u.set_selection_state( unit::SELECTED );
				}
			);
		}
		break;
	default:
		break;
	}
}

void gamemanager::process_mousemoved_event(const sf::Event& event) {

}

void gamemanager::process_mousebuttonpressed_event(const sf::Event& event) {
	switch ( event.mouseButton.button ) {

	case sf::Mouse::Right :
	{

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { // Shift click

			const vector2df destination = window.convertCoords(sf::Mouse::getPosition(window), mapview);

			if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {


				for(unsigned i=0; i<units.size(); ++i) { // editied from r82, due to nested lambda capture issues (N2927)
						if(units[i].is_selected()) {
							//Workaround : if the start and end locations are too close together,
							//search_path returns a path with length=1, and this causes assertion faliures in path
							if(units[i].get_state() == unit::MOVING) {

								if ( units[i].get_destination().distance_to_squared(destination) > get_epsilon()*get_epsilon() ) {
									units[i].append_path(map.search_path(units[i].get_destination(), destination));
								}
							} else {
								if ( units[i].get_position().distance_to_squared(destination) > get_epsilon()*get_epsilon() ) {

									gc::vector2df location = units[i].get_position();
									boost::packaged_task<path> path_task([this, destination, location]() {
										return map.search_path( location, destination );
									});

									boost::shared_future<path> path_future = path_task.get_future();

									boost::thread path_search_thread(std::move(path_task));

									units[i].move_on_future( path_future );
								}
							}

						}
					}
			}
		} else { // Regular click

			const vector2df destination = window.convertCoords(sf::Mouse::getPosition(window), mapview);

			if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {

				for(unsigned i=0; i<units.size(); ++i) { // See line 227 (N2927)
						if ( units[i].is_selected() ) {
							//Workaround : if the start and end locations are too close together,
							//search_path returns a path with length=1, and this causes assertion faliures in path

							if ( units[i].get_position().distance_to_squared(destination) > get_epsilon()*get_epsilon() ) {

								gc::vector2df location = units[i].get_position();
								boost::packaged_task<path> path_task([this, destination, location]() {
									return map.search_path(location, destination );
								});

								boost::shared_future<path> path_future = path_task.get_future();

								boost::thread path_search_thread(std::move(path_task));

								units[i].move_on_future( path_future );
							}
						}
					}
			}

		}
		break;
	}

	case sf::Mouse::Left :
		selection_in_progress = true;
		selection_start = vector2df(window.convertCoords( sf::Mouse::getPosition(window), mapview ));
		break;
	default:
		break;
	}
}

void gamemanager::process_mousebuttonreleased_event(const sf::Event& event) {
	switch ( event.mouseButton.button ) {
	case sf::Mouse::Left :

		//We could start with the mouse down?
		if ( selection_in_progress ) {
			selection_in_progress = false;
			std::for_each( units.begin(), units.end(),
				[this](unit& u) {
					if ( u.get_selection_state() == unit::POTENTIALLY_SELECTED ) {
						u.set_selection_state( unit::SELECTED );
					}
				}
			);
		}
		break;

	default:
		break;
	}
}

void gamemanager::process_mousewheelmoved_event(const sf::Event& event) {

	const sf::Vector2i pos = sf::Mouse::getPosition(window);
	
	const float zoominfactor = 1.f/zoomoutfactor; // See comments below..
	const float maximumzoom = 10.0f;
	const sf::Vector2f rect = mapview.getSize();
	const float w = rect.x;
	const float h = rect.y;
	const volatile float currentzoomfactor = (w / map.get_dimension().x + h / map.get_dimension().y) / 2.f; // Average factors, but should not be difference in them
	
	if ( event.mouseWheel.delta > 0){ // Scroll up!
		if ( currentzoomfactor < maximumzoom ){
			log << logger::all << "Zoom in\n";
			mapview.zoom(zoominfactor); // Zoom in
		    /*  sf::View::zoom changed in SFML 2.0. 
			 *	void sf::View::zoom	(float 	factor)	
			 *	
			 *	Resize the view rectangle relatively to its current size. 
			 *
			 *	Resizing the view simulates a zoom, as the zone displayed on screen grows or shrinks. factor is a multiplier: 
			 *	1 keeps the size unchanged 
			 *	> 1 makes the view bigger (objects appear smaller)  [SIC!]
			 *	< 1 makes the view smaller (objects appear bigger)  [SIC!]
			 */
			// Change center of view to cursor pos
			sf::Vector2f delta = sf::Vector2f( pos - sf::Vector2i(window.getSize()) / 2 );
			const float magicnumber = 0.01f;

			mapview.move(delta * currentzoomfactor * magicnumber);
			sf::Mouse::setPosition(
				sf::Mouse::getPosition() + 
				sf::Vector2i(delta * currentzoomfactor * magicnumber)
			);  // move mouse to where desired object moved.
			

		}
	} else {
		if ( w < map.get_dimension().x || h < map.get_dimension().y) {  // Do not allow infinite zoom out 
			log << logger::all << "Zoom out\n";
			mapview.zoom(zoomoutfactor); // Zoom out
		}
	}

}

void gamemanager::process_textentered_event(const sf::Event& event){
	// Captures non caught keys?
}

void gamemanager::advance(const float frame_rate) {

	const float speed = 100.f;

	for ( unsigned i = 0; i < units.size(); ++i ) {

		units[i].advance(frame_rate);

		if ( units[i].get_state() == unit::MOVING ) {
			//Main path to move on :
			units[i].advance_movement( speed*frame_rate );
		} else {


			//Collision with other units
			for ( unsigned j = i+1; j < units.size(); ++j ) {
				if ( units[j].get_state() != unit::MOVING ) {
					const float radius_sum = units[i].get_radius() + units[j].get_radius();

					//They're too close to each other
					if (units[i].get_position().distance_to_squared(units[j].get_position()) < get_epsilon()*get_epsilon() ) {
						//Move it away in a random direction, so we won't get NAN
						units[i].move( (2.f * get_epsilon() ) * vector2df::random_unit_vector() );
					}

					//TODO the following algorithm is not yet capable of handling units with different traveling speeds

					const float distance_ij_squared = units[i].get_position().distance_to_squared(units[j].get_position());

					if ( distance_ij_squared < radius_sum*radius_sum ) { //They're closer than they should be
						const vector2df move_direction = units[i].get_position() - units[j].get_position();
						const float travel_distance = speed * frame_rate;

						vector2df move_vector;

						const float distance_ij = std::sqrt(distance_ij_squared);
						const float max_travel_distance = radius_sum - distance_ij;

						if ( max_travel_distance < 2.f*travel_distance ) {

//							std::cout << "r1 = " << units[i].get_radius() << ", r2 = " << units[j].get_radius() << ", d = " << std::sqrt(distance_ij_squared) << '"';
//							std::cout << "displacement = " << (radius_sum - std::sqrt(distance_ij_squared)) << "\n\n\n";

							//Add get_epsilon(), because some units get stuck moving away each other by a really small amount, which gets rounded away
							move_vector = (max_travel_distance / 2.f + get_epsilon()) * move_direction.normalize();
						} else {
							move_vector = travel_distance * move_direction.normalize();
						}


						units[i].set_position( units[i].get_position() + move_vector );
						units[j].set_position( units[j].get_position() - move_vector );
					}
				}
			}

			//Collision with closest wall :
			const VisiLibity::distance_point_t closest = VisiLibity::closest_boundary_distance_and_point_squared(
						units[i].get_position().to_visilibity_point(),
						map.get_vis_enviroment() );

			if ( closest.first < units[i].get_radius() * units[i].get_radius() ) {
				const vector2df point_of_impact = vector2df(closest.second);

				//TODO what if we're already in the obstacle?
				//TODO put it a little bit farther away (epsilon problem)
				units[i].move((units[i].get_position() - point_of_impact).normalize() * (units[i].get_radius() - std::sqrt(closest.first) ) );

			}

		}

	}




	if ( selection_in_progress ) {
		const vector2df selection_end = vector2df(window.convertCoords( sf::Mouse::getPosition(window), mapview ));
		std::for_each( units.begin(), units.end(),
			[this, &selection_end](unit& u) {
				if ( u.get_position().is_in_rectangle( selection_start, selection_end ) || // regular, is center in box.
						u.get_position().distance_to_rectangle(selection_start, selection_end) < u.get_radius() ) {
					u.set_selection_state( unit::POTENTIALLY_SELECTED );
				} else {
					u.set_selection_state( unit::UNSELECTED );
				}
			}
		);
	}

}

void gamemanager::draw() {

	window.clear();

	//Draw map related after this
	window.setView(mapview);

	window.draw(map);

	std::for_each( units.begin(), units.end(),
		[this](const unit& u) {
			window.draw(u);
		}
	);

	if ( selection_in_progress ) {

		sf::RectangleShape selection_rectangle( window.convertCoords( sf::Mouse::getPosition(window), mapview ) - selection_start.to_sfml_vector() );

		selection_rectangle.setPosition (selection_start.to_sfml_vector() );
		selection_rectangle.setFillColor( sf::Color(200,0,0,100) );

		window.draw(selection_rectangle);
	}

	//Draw GUI after this
	window.setView(window.getDefaultView());

	//Draw minimap
	game_minimap.render_foreground(units, mapview);
	window.draw(game_minimap);

	sf::Text sfframestr(frame_rate_str, default_font);
	sfframestr.setPosition( vector2df(window_size - vector2di(80, 40)).to_sfml_vector() );
	window.draw( sfframestr );

	window.display();
}


} //namespace gc
