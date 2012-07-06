
#include "gamemanager.hpp"

#include <boost/lexical_cast.hpp>

namespace gc {

gamemanager::gamemanager(unsigned width, unsigned height) :
	window(sf::VideoMode(width, height, 32), "Galaxy Craft"),
	view(sf::Vector2f(200, 300), sf::Vector2f(width/2.f, height/2.f)),
	frame_rate_str("unknown")
{
	polygonf poly1;
	poly1.add_point( vector2df(300, 200) );
	poly1.add_point( vector2df(200, 300) );
	poly1.add_point( vector2df(400, 400) );
	poly1.add_point( vector2df(500, 350) );

	poly1.set_color( sf::Color::Red );

	obstacles.push_back(poly1);
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

    // Move the view using arrow keys
    float Offset = 200.f * window.GetFrameTime();
    if (window.GetInput().IsKeyDown(sf::Key::Up))    view.Move( 0,      -Offset);
    if (window.GetInput().IsKeyDown(sf::Key::Down))  view.Move( 0,       Offset);
    if (window.GetInput().IsKeyDown(sf::Key::Left))  view.Move(-Offset,  0);
    if (window.GetInput().IsKeyDown(sf::Key::Right)) view.Move( Offset,  0);

    // Zoom and unzoom using + and - keys
    if (window.GetInput().IsKeyDown(sf::Key::Add))      view.Zoom(1.001f);
    if (window.GetInput().IsKeyDown(sf::Key::Subtract)) view.Zoom(0.999f);

}

void gamemanager::advance(const float frame_rate) {


}

void gamemanager::draw() {
	window.Clear();

	window.SetView(view);

	for ( unsigned i = 0; i < obstacles.size(); ++i ) {
		obstacles[i].draw(window);
	}

	window.SetView(window.GetDefaultView());

	window.Draw( sf::String(frame_rate_str) );

	window.Display();
}


} //namespace gc
