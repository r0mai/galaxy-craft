
#include "gamemanager.hpp"
#include "vector2d.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <SFML/Audio.hpp>
#include <boost/lexical_cast.hpp>
#include "object.hpp"

namespace gc {

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
	units.push_back( unit( vector2df(350.0,250.0), manager.getimage("obj.png") ) );
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
		{

			switch ( event.MouseButton.Button ) {

			case sf::Mouse::Left :
				std::for_each( units.begin(), units.end(),
					[this, &event](unit& u) {
						u.move_on( map.search_path( units[0].get_position(),
								vector2df(window.ConvertCoords( event.MouseButton.X, event.MouseButton.Y, &mapview ) )) );
					}
				);
				break;
			default:
				break;
			}
			break;
		}
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
	std::for_each( units.begin(), units.end(),
		[this, frame_rate](unit& u) {
			u.advance( frame_rate * 400.f );
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


	//Draw GUI after this
	window.SetView(window.GetDefaultView());

	sf::String sfframestr(frame_rate_str);

	sfframestr.SetPosition( vector2df(window_size - vector2di(80, 40)).to_sfml_vector() );

	window.Draw( sfframestr );

	window.Display();
}


} //namespace gc
