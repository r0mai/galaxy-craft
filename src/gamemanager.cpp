
#include "gamemanager.hpp"

#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>

namespace gc {

gamemanager::gamemanager(unsigned width, unsigned height) :
	window(sf::VideoMode(width, height, 32), "Galaxy Craft"),
	window_size(width, height),
	frame_rate_str("unknown")
{
	init();
}

void gamemanager::init() {
	readmap("test.map");

	test_path = path::search_path( vector2df(250, 150), vector2df(650, 650), obstacles );
}

void gamemanager::readmap(const std::string& mapfile) {
	std::ifstream in(mapfile.c_str());

	std::string line;

	std::getline( in, line );
	std::stringstream ss(line);
	//TODO syntax checking or xml parsing
	ss >> map_size.x >> map_size.y;

	mapview = sf::View(sf::Vector2f(map_size.x/2.f, map_size.y/2.f), sf::Vector2f(window_size.x/2.f, window_size.y/2.f));

	while ( std::getline( in, line ) ) {
		std::stringstream ss2(line);

		polygonf poly;
		float x, y;
		while ( ss2 >> x && ss2 >> y ) {
			poly.add_point( vector2df(x, y) );
		}
		poly.set_color( sf::Color::Red );

		if ( poly.get_points().size() >= 3 ) {
			obstacles.push_back( poly );
		}

	}


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

				test_path = path::search_path(
						window.ConvertCoords( window.GetInput().GetMouseX(), window.GetInput().GetMouseY(), &mapview ),
						test_path.get_end(),
						obstacles );
				break;
			case sf::Mouse::Right :
				test_path = path::search_path(
						test_path.get_start(),
						window.ConvertCoords( window.GetInput().GetMouseX(), window.GetInput().GetMouseY(), &mapview ),
						obstacles );
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	//const int mousex = window.GetInput().GetMouseX();
	//const int mousey = window.GetInput().GetMouseY();

    // Move the view using arrow keys
    float Offset = 200.f * window.GetFrameTime();
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

	window.Draw( sf::Shape::Rectangle( sf::Vector2f(0.f,0.f), map_size.to_sfml_vector(), sf::Color(20, 20, 20) ) );

	for ( unsigned i = 0; i < obstacles.size(); ++i ) {
		obstacles[i].draw(window);
	}

	test_path.draw(window);

	//Draw GUI after this
	window.SetView(window.GetDefaultView());

	sf::String sfframestr(frame_rate_str);

	sfframestr.SetPosition( vector2df(window_size - vector2di(80, 40)).to_sfml_vector() );

	window.Draw( sfframestr );

	window.Display();
}


} //namespace gc
