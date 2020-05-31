#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "GameWindow.h"

int main() {
	try {
		GameWindow game_window;
		game_window.gameLoop();
	}
	catch (const std::invalid_argument& ex){
		std::cout << ex.what();
	}
	return 0;
}