
#include <cstdlib>
#include <ctime>

#include "gamemanager.hpp"

#include "vector2d.hpp"
#include <SFML/Graphics.hpp>

int main() {
	
	std::srand( std::time(0) );

	gc::gamemanager game(800, 600);

	game.run();
	

}
/*

int main(){
    // Create the main rendering window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML Graphics");

    // Load the sprite image from a file
    sf::Image Image;
    if (!Image.LoadFromFile("obj.png"))
        return EXIT_FAILURE;

    // Create the sprite
    sf::Sprite Sprite(Image);

    // Change its properties
    Sprite.SetColor(sf::Color(0, 255, 255, 128));
    Sprite.SetPosition(200.f, 100.f);

	Sprite.SetCenter(Sprite.GetSize() / 2.f);
    Sprite.SetScale(2.f, 2.f);

	

    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }

        // Get elapsed time
        float ElapsedTime = App.GetFrameTime();

        // Move the sprite
        if (App.GetInput().IsKeyDown(sf::Key::Left))  Sprite.Move(-100 * ElapsedTime, 0);
        if (App.GetInput().IsKeyDown(sf::Key::Right)) Sprite.Move( 100 * ElapsedTime, 0);
        if (App.GetInput().IsKeyDown(sf::Key::Up))    Sprite.Move(0, -100 * ElapsedTime);
        if (App.GetInput().IsKeyDown(sf::Key::Down))  Sprite.Move(0,  100 * ElapsedTime);

        // Rotate the sprite
        if (App.GetInput().IsKeyDown(sf::Key::Add))      Sprite.Rotate(- 100 * ElapsedTime);
        if (App.GetInput().IsKeyDown(sf::Key::Subtract)) Sprite.Rotate(+ 100 * ElapsedTime);

        // Clear screen
        App.Clear();

        // Display sprite in our window
        App.Draw(Sprite);

        // Display window contents on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}

	
	*/