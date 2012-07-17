
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
