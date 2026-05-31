#include <iostream>
#include "Game.h"

using namespace std;
using namespace sf;
 
int main() {

	// init game engine 

	Game game;
	// Game loop 

	while(game.running()) {

		// update
		game.update();
	
		// render
		game.render();

		}

// end of game

	return 0;
}