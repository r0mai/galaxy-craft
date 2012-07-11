
#include "gamemanager.hpp"
#include "vector2d.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include <SFML/Audio.hpp>
#include <boost/lexical_cast.hpp>
#include "object.hpp"

namespace gc {

gamemanager::gamemanager(unsigned width, unsigned height) :
	window(sf::VideoMode(width, height, 32), "Galaxy Craft"),
	window_size(width, height),
	map(gamemap::from_file( "test.map" )),
	mapview(sf::Vector2f(map.get_dimension().x/2.f, map.get_dimension().y/2.f), sf::Vector2f(window_size.x/2.f, window_size.y/2.f)),
	selection_in_progress(false),
	frame_rate_str("unknown")
{
	init();
}



void gamemanager::init() {
	test_unit = unit( vector2df(350.0,250.0), manager.getimage("obj.png") );
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
		default:
			break;
		}
	}

	float Offset = 400.f * window.GetFrameTime();

    // Move the view using arrow keys
    if (window.GetInput().IsKeyDown(sf::Key::Up))    mapview.Move( 0,      -Offset);
    if (window.GetInput().IsKeyDown(sf::Key::Down))  mapview.Move( 0,       Offset);
    if (window.GetInput().IsKeyDown(sf::Key::Left))  mapview.Move(-Offset,  0);
    if (window.GetInput().IsKeyDown(sf::Key::Right)) mapview.Move( Offset,  0);

    // Zoom and unzoom using + and - keys
    if (window.GetInput().IsKeyDown(sf::Key::Add))      mapview.Zoom(1.001f);
    if (window.GetInput().IsKeyDown(sf::Key::Subtract)) mapview.Zoom(0.999f);

}

void gamemanager::process_keypressed_event(const sf::Event& event) {
	switch ( event.Key.Code ) {
	case sf::Key::Escape :
		window.Close();
		break;
	case sf::Key::Space :
		units.push_back( unit( vector2df(window.ConvertCoords( window.GetInput().GetMouseX(),
				window.GetInput().GetMouseY(), &mapview )), manager.getimage("obj.png") ) );
		break;
	default:
		break;
	}
}

void gamemanager::process_mousemoved_event(const sf::Event& event) {
	//This line is fun! :D
	//test_unit.move_on( map.search_path( test_unit.get_position(),	vector2df(window.ConvertCoords( event.MouseMove.X, event.MouseMove.Y, &mapview ) )) );

	test_unit.set_position( vector2df(window.ConvertCoords( event.MouseMove.X, event.MouseMove.Y, &mapview ) ) );

	VisiLibity::distance_point_t closest =
			VisiLibity::closest_boundary_distance_and_point_squared( test_unit.get_position().to_visilibity_point(), map.get_vis_enviroment() );

	test_point = closest.second;

}

void gamemanager::process_mousebuttonpressed_event(const sf::Event& event) {
	switch ( event.MouseButton.Button ) {

	case sf::Mouse::Right :
	{
		std::clock_t start = std::clock();
		unsigned i = 0;
		std::for_each( units.begin(), units.end(),
			[this, &event, &i](unit& u) {
				if ( u.is_selected() ) {
					++i;
					u.move_on( map.search_path( u.get_position(),
						vector2df(window.ConvertCoords( event.MouseButton.X, event.MouseButton.Y, &mapview ) )) );
				}
			}
		);

		std::cout << "Path planning took " << (clock() - start)/float(CLOCKS_PER_SEC) << "s for " << i << " units" << std::endl;
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

void gamemanager::advance(const float frame_rate) {
	std::for_each( units.begin(), units.end(),
		[this, frame_rate](unit& u) {
			u.advance( frame_rate * 100.f );
		}
	);
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

	test_unit.draw(window);
	window.Draw( sf::Shape::Circle(test_point.to_sfml_vector(), 3.f, sf::Color::Green) );

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
