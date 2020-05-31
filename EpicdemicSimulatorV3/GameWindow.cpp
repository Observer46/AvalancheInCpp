#include "GameWindow.h"

GameWindow::GameWindow(const size_t& initial_width, const size_t& initial_height) :
	_game_logic(),
	_game_window(sf::VideoMode(initial_width, initial_height), "Avalanche!"),
	_play_button("Play!", sf::Vector2f(Button::slot_x, Button::slot1_y)), _exit_button("Exit", sf::Vector2f(Button::slot_x, Button::slot2_y)),
	_reset_button("Reset", sf::Vector2f(Button::slot_x, Button::slot1_y)), _back_to_menu_button("Back to menu", sf::Vector2f(Button::slot_x, Button::slot2_y)),
	_title(), _mode{ WindowMode::Menu }, _score_font{}, _score_text(), _mouse_particles(1000, 2.f)
{
	this->_game_window.setFramerateLimit(60);		// 60 fps
	
	if (!this->_score_font.loadFromFile(Button::default_font_name))
		throw std::invalid_argument("There is no such font here!");
	
	this->_score_text.setFont(this->_score_font);
	this->_score_text.setFillColor(sf::Color::Black);
	this->_score_text.setCharacterSize(40);
};

void GameWindow::gameLoop()
{
	sf::Clock clock;
	sf::Event event;

	this->_exit_button.setVisible(this->_game_logic.getView().getCenter());
	this->_play_button.setVisible(this->_game_logic.getView().getCenter());
	
	while (this->_game_window.isOpen()) {
		while (this->_game_window.pollEvent(event)) 			// Processing all pending events
			this->handleEvent(event);

		this->whichButtonClicked();	

		sf::Time delta = clock.restart();
		sf::Vector2i mouse = sf::Mouse::getPosition(this->_game_window);
		this->_mouse_particles.set_emitter(this->_game_window.mapPixelToCoords(mouse));
		
		if (this->_mode == WindowMode::Game) {
			if (this->_game_logic.isPlayerAlive()) {
				sf::Vector2u window_size = this->_game_window.getSize();
				this->_game_logic.update(delta, window_size);
				this->updateScore();
				this->_game_window.setView(this->_game_logic.getView());
			}
			else {			// Can be thrown elsewhere
				this->_mouse_particles.update(delta);
				this->_reset_button.setVisible(this->_game_logic.getView().getCenter());
				this->_back_to_menu_button.setVisible(this->_game_logic.getView().getCenter());
				this->showEndScore();
			}
		}
		else {
			this->_title.update(delta, this->_game_logic.getView().getCenter());
			this->_mouse_particles.update(delta);
		}
		
		this->_game_window.clear(sf::Color::White);
		this->drawAll();
		this->_game_window.display();
	}
}

void GameWindow::drawButtons()
{
	Button all_buttons[] = { this->_back_to_menu_button, this->_exit_button, this->_play_button, this->_reset_button };
	for (auto& button : all_buttons)
		this->_game_window.draw(button);
}

void GameWindow::drawAll()
{
	if (this->_mode == WindowMode::Game) {
		this->_game_window.draw(this->_game_logic.getPlayer());
		for (const auto& obst : this->_game_logic.getObstacles())
			this->_game_window.draw(obst);
		this->_game_window.draw(this->_game_logic.getLava());
		this->_game_window.draw(this->_score_text);
		if (!this->_game_logic.isPlayerAlive())
			this->_game_window.draw(this->_mouse_particles);
	}
	else {
		this->_game_window.draw(this->_title);
		this->_game_window.draw(this->_mouse_particles);
	}

	this->drawButtons();

}

void GameWindow::handleEvent(const sf::Event& event)
{
	switch (event.type) {
	case sf::Event::Closed:
		this->closeWindow();
		break;
	case sf::Event::Resized:
		this->resize(sf::Vector2u(event.size.width, event.size.height));
		break;
	case sf::Event::MouseMoved:
		this->checkWhichButton();
		break;
	case sf::Event::MouseButtonReleased:
		this->whichButtonReleased();
	default:
		break;
	}
}

void GameWindow::checkWhichButton()
{
	sf::Vector2i rel_mouse_pos =  sf::Mouse::getPosition(this->_game_window);
	sf::Vector2f mouse_pos = this->_game_window.mapPixelToCoords(rel_mouse_pos);

	if (this->_play_button.isOnScreen() && this->mouseOnButton(this->_play_button, mouse_pos))
		this->_play_button.changeButtonMode(ButtonMode::MouseOn);
	else
		this->_play_button.changeButtonMode(ButtonMode::Unclicked);

	if (this->_exit_button.isOnScreen() && this->mouseOnButton(this->_exit_button, mouse_pos)) 
		this->_exit_button.changeButtonMode(ButtonMode::MouseOn);
	else
		this->_exit_button.changeButtonMode(ButtonMode::Unclicked);

	if (this->_reset_button.isOnScreen() && this->mouseOnButton(this->_reset_button, mouse_pos)) 
		this->_reset_button.changeButtonMode(ButtonMode::MouseOn);
	else
		this->_reset_button.changeButtonMode(ButtonMode::Unclicked);

	if (this->_back_to_menu_button.isOnScreen() && this->mouseOnButton(this->_back_to_menu_button, mouse_pos)) 
		this->_back_to_menu_button.changeButtonMode(ButtonMode::MouseOn);
	else
		this->_back_to_menu_button.changeButtonMode(ButtonMode::Unclicked);
}

bool GameWindow::mouseOnButton(Button& button, const sf::Vector2f& mouse_pos) const
{
	sf::FloatRect button_shape = button.getGlobBound();
	return button_shape.contains(mouse_pos);			// Checks if mouse is inside button
}

void GameWindow::whichButtonReleased()
{
	sf::Vector2i rel_mouse_pos = sf::Mouse::getPosition(this->_game_window);
	sf::Vector2f mouse_pos = this->_game_window.mapPixelToCoords(rel_mouse_pos);

	if (this->_play_button.isOnScreen() && this->mouseOnButton(this->_play_button, mouse_pos)) 
		this->beginGame();

	if (this->_exit_button.isOnScreen() && this->mouseOnButton(this->_exit_button, mouse_pos)) 
		this->closeWindow();

	if (this->_reset_button.isOnScreen() && this->mouseOnButton(this->_reset_button, mouse_pos)) 
		this->beginGame();

	if (this->_back_to_menu_button.isOnScreen() && this->mouseOnButton(this->_back_to_menu_button, mouse_pos)) 
		this->backToMenu();
}

void GameWindow::whichButtonClicked()
{
	sf::Vector2i rel_mouse_pos = sf::Mouse::getPosition(this->_game_window);
	sf::Vector2f mouse_pos = this->_game_window.mapPixelToCoords(rel_mouse_pos);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (this->_play_button.isOnScreen() && this->mouseOnButton(this->_play_button, mouse_pos))
			this->_play_button.changeButtonMode(ButtonMode::Clicked);

		if (this->_exit_button.isOnScreen() && this->mouseOnButton(this->_exit_button, mouse_pos))
			this->_exit_button.changeButtonMode(ButtonMode::Clicked);

		if (this->_reset_button.isOnScreen() && this->mouseOnButton(this->_reset_button, mouse_pos))
			this->_reset_button.changeButtonMode(ButtonMode::Clicked);

		if (this->_back_to_menu_button.isOnScreen() && this->mouseOnButton(this->_back_to_menu_button, mouse_pos))
			this->_back_to_menu_button.changeButtonMode(ButtonMode::Clicked);
	}
}

void GameWindow::beginGame() {
	this->_play_button.setInvisible();
	this->_exit_button.setInvisible();
	this->_reset_button.setInvisible();
	this->_back_to_menu_button.setInvisible();
	this->_mode = WindowMode::Game;
	GameLogic game_logic;
	this->_game_logic = game_logic;		//Reset whole game
}

void GameWindow::backToMenu() {
	this->_reset_button.setInvisible();
	this->_back_to_menu_button.setInvisible();
	this->_play_button.setVisible(this->_game_logic.getView().getCenter());
	this->_exit_button.setVisible(this->_game_logic.getView().getCenter());
	this->_mode = WindowMode::Menu;
}

void GameWindow::showEndScore() {
	sf::Vector2f text_size = sf::Vector2f(this->_score_text.getGlobalBounds().width, this->_score_text.getGlobalBounds().height);
	sf::Vector2f view_center = this->_game_logic.getView().getCenter();
	this->_score_text.setPosition(sf::Vector2f(view_center.x - text_size.x / 2.f, view_center.y - GameLogic::initial_window_height / 3));
}

void GameWindow::updateScore()
{
	this->_score_text.setString("SCORE: " + std::to_string(this->_game_logic.getScore()));
	sf::Vector2f view_center = this->_game_logic.getView().getCenter();
	this->_score_text.setPosition(sf::Vector2f(view_center.x - 2 * GameLogic::initial_window_width / 5,
		view_center.y - 2 * GameLogic::initial_window_height / 5));
}
