
#include "gamemanager.hpp"

#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>

namespace gc {

gamemanager::gamemanager(unsigned width, unsigned height) :
	window(sf::VideoMode(width, height, 32), "Galaxy Craft"), window_width(width), window_height(height),
	/*view(sf::Vector2f(width/2.f, height/2.f), sf::Vector2f(width/2.f, height/2.f)),*/
	frame_rate_str("unknown")
{
	init();

/*
	polygonf poly1;
	poly1.add_point( vector2df(300, 200) );
	poly1.add_point( vector2df(200, 300) );
	poly1.add_point( vector2df(400, 400) );
	poly1.add_point( vector2df(500, 350) );

	poly1.set_color( sf::Color::Red );

	obstacles.push_back(poly1);
*/
}

void gamemanager::init() {
	readmap("test.map");
}

void gamemanager::readmap(const std::string& mapfile) {
	std::ifstream in(mapfile.c_str());

	std::string line;

	std::getline( in, line );
	std::stringstream ss(line);
	//TODO syntax checking or xml parsing
	ss >> map_width >> map_height;

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
		frame_rate_str = boost::lexical_cast<std::string>(1.f/frame_rate);

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
		default:
			break;
		}
	}

	const int mousex = window.GetInput().GetMouseX();
	const int mousey = window.GetInput().GetMouseY();

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

	window.SetView(mapview);

	for ( unsigned i = 0; i < obstacles.size(); ++i ) {
		obstacles[i].draw(window);
	}

	window.SetView(window.GetDefaultView());

	window.Draw( sf::String(frame_rate_str) );

	window.Display();
}


} //namespace gc
