
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include "gamemanager.hpp"
#include "vector2d.hpp"

int main() {

    std::srand( std::time(0) );

	gc::gamemanager game;

	game.run();
	

}
