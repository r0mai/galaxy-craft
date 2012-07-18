
#include "gamemanager.hpp"
#include "vector2d.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <SFML/Audio.hpp>
#include <boost/lexical_cast.hpp>
#include "object.hpp"

namespace gc {

const float gamemanager::unit_size = 13.f; //Can be tweaked for bigger units

gamemanager::gamemanager(unsigned width, unsigned height) :
	window(sf::VideoMode(width, height, 32), "Galaxy Craft"),
	window_size(width, height),
	map(gamemap::from_file( "test.map", unit_size * 0.8f )), //(0.8f) : magic number, can be tweaked if path searching is buggy <1 seems to work better
	mapview(sf::Vector2f(map.get_dimension().x/2.f, map.get_dimension().y/2.f), sf::Vector2f(window_size.x/2.f, window_size.y/2.f)),
	selection_in_progress(false),
	is_mouse_in_focus(true),
	frame_rate_str("unknown")
{
	init();
}



void gamemanager::init() {

}

void gamemanager::run() {

	while(window.IsOpened()) {
		const float frame_rate = window.GetFrameTime();
		frame_rate_str = boost::lexical_cast<std::string>( static_cast<int>(1.f/frame_rate) );

        process_events();
        advance(frame_rate);
        draw();
	}

}

void gamemanager::process_events() {
	sf::Event event;
	while( window.GetEvent( event ) ) {
		switch ( event.Type ) {
		case sf::Event::Closed :
			window.Close();
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

	float Offset = 400.f * window.GetFrameTime();

	const float rim = 0.08f;
	const float upthreshold = window.GetHeight()   * rim;
	const float downthreshold = window.GetHeight() * (1-rim);
	const float leftthreshold = window.GetWidth()  * rim;
	const float rightthreshold = window.GetWidth() * (1-rim);

	
    // Move the view using arrow keys or by mouse position
	// TODO: add check for out of rangness, and is window in focus

	if ( window.GetInput().IsKeyDown(sf::Key::Up)    || (window.GetInput().GetMouseY() < upthreshold    && is_mouse_in_focus  ) ) {
		mapview.Move( 0, -Offset);
	}
    if ( window.GetInput().IsKeyDown(sf::Key::Down)  || (window.GetInput().GetMouseY() > downthreshold  && is_mouse_in_focus  ) ) {
		mapview.Move( 0, Offset);
    }
	if ( window.GetInput().IsKeyDown(sf::Key::Left)  || (window.GetInput().GetMouseX() < leftthreshold  && is_mouse_in_focus  ) ) {
		mapview.Move(-Offset, 0);
	}
	if ( window.GetInput().IsKeyDown(sf::Key::Right) || (window.GetInput().GetMouseX() > rightthreshold && is_mouse_in_focus  ) ) {
		mapview.Move( Offset, 0);
	}

	

}

void gamemanager::process_keypressed_event(const sf::Event& event) {
	switch ( event.Key.Code ) {
	case sf::Key::Escape :
		window.Close();
		break;
	case sf::Key::Space :
		{

			const vector2df destination = vector2df(window.ConvertCoords( window.GetInput().GetMouseX(),
					window.GetInput().GetMouseY(), &mapview ));
			if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {
				if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {
					units.push_back( unit( destination, unit_size, manager.getimage("obj.png") ) );
				}
			}
		}
		break;
	case sf::Key::R:
		std::for_each(units.begin(), units.end(), [](unit& u){u.set_orientation(M_PI/4);} );
		break;
	case sf::Key::A:
		if ( window.GetInput().IsKeyDown(sf::Key::LControl) ) {// CTRL A
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
	switch ( event.MouseButton.Button ) {

	case sf::Mouse::Right :
	{
		if ( window.GetInput().IsKeyDown(sf::Key::LShift)) { // Shift click

			const vector2df destination = window.ConvertCoords(window.GetInput().GetMouseX(), window.GetInput().GetMouseY(), &mapview);

			if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {


				std::for_each(units.begin(), units.end(),
					[this, &destination](unit& u) {
						if(u.is_selected()) {
							//Workaround : if the start and end locations are too close together,
							//search_path returns a path with length=1, and this causes assertion faliures in path
							if(u.get_state() == unit::MOVING) {

								if ( u.get_destination().distance_to_squared(destination) > get_epsilon()*get_epsilon() ) {
									u.append_path(map.search_path(u.get_destination(), destination));
								}
							} else {
								if ( u.get_position().distance_to_squared(destination) > get_epsilon()*get_epsilon() ) {
									u.move_on( map.search_path( u.get_position(), destination ) );
								}
							}

						}
					}
				);

			}
		} else { // Regular click

			const vector2df destination = window.ConvertCoords(window.GetInput().GetMouseX(), window.GetInput().GetMouseY(), &mapview);

			if ( destination.to_visilibity_point().in( map.get_vis_enviroment_offset() ) ) {

				std::for_each( units.begin(), units.end(),
					[this, &event, &destination](unit& u) {
						if ( u.is_selected() ) {
							//Workaround : if the start and end locations are too close together,
							//search_path returns a path with length=1, and this causes assertion faliures in path

							if ( u.get_position().distance_to_squared(destination) > get_epsilon()*get_epsilon() ) {
								u.move_on( map.search_path( u.get_position(), destination ) );
							}
						}
					}
				);
			}

		}
		break;
	}

	case sf::Mouse::Left :
		selection_in_progress = true;
		selection_start = vector2df(window.ConvertCoords( event.MouseButton.X, event.MouseButton.Y, &mapview ));
		break;
	default:
		break;
	}
}

void gamemanager::process_mousebuttonreleased_event(const sf::Event& event) {
	switch ( event.MouseButton.Button ) {
	case sf::Mouse::Left :
	{
		selection_in_progress = false;
		const vector2df selection_end = vector2df(window.ConvertCoords( event.MouseButton.X, event.MouseButton.Y, &mapview ));
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
	
	const float zoominfactor = 1.05f;
	const float zoomoutfactor = 0.9523f;
	const float maximumzoom = 10.0f;
	const sf::FloatRect rect = mapview.GetRect();
	const float w = rect.GetWidth();
	const float h = rect.GetHeight(); 

	if ( event.MouseWheel.Delta > 0){
		if ( w > (map.get_dimension().x / maximumzoom) && h > (map.get_dimension().y / maximumzoom ) ) {
			mapview.Zoom(zoominfactor); // Zoom in
		}
	} else {
		if ( w < map.get_dimension().x || h < map.get_dimension().y) {  // Do not allow infinite zoom out
			mapview.Zoom(zoomoutfactor); // Zoom out
		}
	}

}

void gamemanager::advance(const float frame_rate) {

	const float speed = 100.f;

	for ( unsigned i = 0; i < units.size(); ++i ) {



		if ( units[i].get_state() == unit::MOVING ) {
			//Main path to move on :
			units[i].advance( speed*frame_rate );
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
	window.Clear();

	//Draw map related after this
	window.SetView(mapview);

	map.draw(window);
	
	std::for_each( units.begin(), units.end(),
		[this](const unit& u) {
			u.draw(window);
		}
	);

	if ( selection_in_progress ) {
		window.Draw( sf::Shape::Rectangle( selection_start.to_sfml_vector(),
				vector2df( window.ConvertCoords( window.GetInput().GetMouseX(),
						window.GetInput().GetMouseY(), &mapview ) ).to_sfml_vector(),
				sf::Color(200,0,0,100) ) );
	}

	//Draw GUI after this
	window.SetView(window.GetDefaultView());

	sf::String sfframestr(frame_rate_str);
	sfframestr.SetPosition( vector2df(window_size - vector2di(80, 40)).to_sfml_vector() );
	window.Draw( sfframestr );

	window.Display();
}


} //namespace gc
