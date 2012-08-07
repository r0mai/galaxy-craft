
#include "gamemanager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

#include <SFML/Audio.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "vector2d.hpp"
#include "object.hpp"
#include "configreader.hpp"

#include <boost/version.hpp>


namespace gc {


gamemanager::gamemanager() {
	init();
	log = logger(logger::all);
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
	

	window.create( sf::VideoMode(window_width, window_height, 32), window_title );
	window_size = vector2di(window_width, window_height);

	map = gamemap::from_file( map_file, unit_size * obstacle_offset_ratio );

	mapview = sf::View(sf::Vector2f(map.get_dimension().x/2.f, map.get_dimension().y/2.f), sf::Vector2f(window_size.x/2.f, window_size.y/2.f));

	selection_in_progress = false;
	is_mouse_in_focus = true;
	frame_rate_str = "unknown";



}

void gamemanager::run() {

	sf::Clock frame_clock;

	while(window.isOpen()) {
		const float frame_rate = frame_clock.restart().asSeconds();
		frame_rate_str = boost::lexical_cast<std::string>( static_cast<int>(1.f/frame_rate) );

		std::clock_t start = std::clock();

        process_events(frame_rate);
        std::clock_t process_events_time = std::clock() - start;
        start = std::clock();

        advance(frame_rate);
        std::clock_t advance_time = std::clock() - start;
        start = std::clock();

        draw();
        std::clock_t draw_time = std::clock() - start;

        //std::cout << "process_events : " << process_events_time << "\n";
        //std::cout << "advance_time : " << advance_time << "\n";
        //std::cout << "draw_time : " << draw_time << "\n\n";
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
		case sf::Event::MouseEntered:
			is_mouse_in_focus = true;
			break;
		case sf::Event::MouseLeft:
			is_mouse_in_focus = false;
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
					units.push_back( unit( destination, unit_size, manager.get_texture("obj.png") ) );
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
					u.set_selected(true);
				}
			);
		}
		break;
	default:
		break;
	}
}

void gamemanager::process_mousemoved_event(const sf::Event& event) {
	//This line is fun! :D
	//test_unit.move_on( map.search_path( test_unit.get_position(),	vector2df(window.ConvertCoords( event.MouseMove.X, event.MouseMove.Y, &mapview ) )) );
#if 0
	test_unit.set_position( vector2df(window.ConvertCoords( event.MouseMove.X, event.MouseMove.Y, &mapview ) ) );

	VisiLibity::distance_point_t closest =
			VisiLibity::closest_boundary_distance_and_point_squared( test_unit.get_position().to_visilibity_point(), map.get_vis_enviroment() );

	test_point = closest.second;
#endif

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
	{
		selection_in_progress = false;
		const vector2df selection_end = vector2df(window.convertCoords( sf::Mouse::getPosition(window), mapview ));
		std::for_each( units.begin(), units.end(),
			[this, &selection_end](unit& u) {

				u.set_selected( u.get_position().is_in_rectangle( selection_start, selection_end ) );

			}
		);
		break;
	}
	default:
		break;
	}
}

void gamemanager::process_mousewheelmoved_event(const sf::Event& event) {
	
	const float zoomoutfactor = 1.05f;
	const float zoominfactor = 1.f/zoomoutfactor; // See comments below..
	const float maximumzoom = 10.0f;
	const sf::Vector2f rect = mapview.getSize();
	const float w = rect.x;
	const float h = rect.y;
	const float currentzoomfactor = (w / map.get_dimension().x + h / map.get_dimension().y) / 2.f; // Average factors
	
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

		}
	} else {
		if ( w < map.get_dimension().x || h < map.get_dimension().y) {  // Do not allow infinite zoom out
			log << logger::all << "Zoom out\n";
			mapview.zoom(zoomoutfactor); // Zoom out
		}
	}

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

					if ( units[i].get_position().distance_to_squared(units[j].get_position()) < radius_sum*radius_sum ) { //They're closer than they should be
						vector2df move_vector = speed*frame_rate*( units[i].get_position() - units[j].get_position() ).normalize();

						/*
						const float max_distance = units[i].radius + units[j].radius - units[i].get_position().distance_to_squared(units[j].get_position());

						if ( move_vector.length() >= max_distance ) {
							move_vector *= max_distance/2.f;
						} else {

						}

						moving_vector += move_vector;
						units[j].pos += -1.f*move_vector;
						*/

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

}

void gamemanager::draw() {
	window.clear();

	//Draw map related after this
	window.setView(mapview);

	map.draw(window);
	
	std::for_each( units.begin(), units.end(),
		[this](const unit& u) {
			u.draw(window);
		}
	);

	if ( selection_in_progress ) {

		sf::RectangleShape selection_rectangle( window.convertCoords( sf::Mouse::getPosition(window), mapview ) - selection_start.to_sfml_vector() );

		selection_rectangle.setPosition (selection_start.to_sfml_vector() );
		selection_rectangle.setFillColor( sf::Color(200,0,0,100) );

		window.draw(selection_rectangle);;
	}

	//Draw GUI after this
	window.setView(window.getDefaultView());



/*
	sf::Text sfframestr(frame_rate_str);
	sfframestr.setPosition( vector2df(window_size - vector2di(80, 40)).to_sfml_vector() );
	window.draw( sfframestr );
*/
	window.display();
}


} //namespace gc
