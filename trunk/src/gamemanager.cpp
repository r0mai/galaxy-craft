
#include "gamemanager.hpp"
#include "vector2d.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/Audio.hpp>
#include <boost/lexical_cast.hpp>
#include "object.hpp"

namespace gc {


	// Object testing here! Yes I know, no global var usage..
	object obj("obj.png", vector2df(150.0,250.0), 25.0);

gamemanager::gamemanager(unsigned width, unsigned height) :
	window(sf::VideoMode(width, height, 32), "Galaxy Craft"),
	window_size(width, height),
	map(gamemap::from_file( "test.map" )),
	mapview(sf::Vector2f(map.get_dimension().x/2.f, map.get_dimension().y/2.f), sf::Vector2f(window_size.x/2.f, window_size.y/2.f)),
	frame_rate_str("unknown")
{
	init();
}



void gamemanager::init() {

	test_path = map.search_path( vector2df(250, 150), vector2df(650, 650) );
	
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
			switch ( event.Key.Code ) {
			case sf::Key::Escape :
				window.Close();
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed :
			switch ( event.MouseButton.Button ) {
			case sf::Mouse::Left :

				test_path = map.search_path(
						window.ConvertCoords( window.GetInput().GetMouseX(), window.GetInput().GetMouseY(), &mapview ),
						test_path.get_end() );
				break;
			case sf::Mouse::Right :
				test_path = map.search_path(
						test_path.get_start(),
						window.ConvertCoords( window.GetInput().GetMouseX(), window.GetInput().GetMouseY(), &mapview ) );
				break;
			default:
				break;
			}
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

void gamemanager::advance(const float frame_rate) {


}

void gamemanager::draw() {
	window.Clear();

	//Draw map related after this
	window.SetView(mapview);

	map.draw(window);
	test_path.draw(window);

	//Draw GUI after this
	window.SetView(window.GetDefaultView());

	sf::String sfframestr(frame_rate_str);

	sfframestr.SetPosition( vector2df(window_size - vector2di(80, 40)).to_sfml_vector() );

	window.Draw( sfframestr );

	obj.draw(window);

	window.Display();
}


} //namespace gc
