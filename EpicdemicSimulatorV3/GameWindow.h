#pragma once
#include <SFML/Graphics.hpp>

#include "ParticleSystem.h"
#include "GameLogic.h"
#include "Button.h"
#include "title.h"

enum class WindowMode {
	Menu,
	Game
};

class GameWindow
{
public:
	GameWindow(const size_t& initial_width = GameLogic::initial_window_width, const size_t& initial_height = GameLogic::initial_window_height);

	unsigned int getWidth() const { return this->_game_window.getSize().x; }
	unsigned int getHeight() const { return this->_game_window.getSize().y; }
	void gameLoop();

	void closeWindow() { this->_game_window.close(); }
	void resize(const sf::Vector2u& resized);		// TO DELETE
	void drawButtons();
	void drawAll();

	void handleEvent(const sf::Event& event);
	void checkWhichButton();
	bool mouseOnButton(Button& button, const sf::Vector2f& mouse_pos) const;
	void whichButtonClicked();
	void whichButtonReleased();

	void beginGame();
	void backToMenu();
	void showEndScore();
	void updateScore();

private:
	GameLogic _game_logic;
	sf::RenderWindow _game_window;
	Button _play_button;
	Button _exit_button;
	Button _reset_button;
	Button _back_to_menu_button;
	Title _title;
	WindowMode _mode;
	sf::Font _score_font;
	sf::Text _score_text;
	ParticleSystem _mouse_particles;
};

