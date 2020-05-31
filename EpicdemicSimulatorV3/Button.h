#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "GameLogic.h"

enum class ButtonMode {
	Unclicked,
	MouseOn,
	Clicked
};

class Button : public sf::Drawable, public sf::Transformable
{
public:
	Button(const std::string& title, const sf::Vector2f& position,
		const sf::Vector2f& size = sf::Vector2f(GameLogic::initial_window_width / 3, GameLogic::initial_window_height / 12));

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void changeButtonMode(const ButtonMode& mode) { this->_mode = mode; }
	bool isOnScreen() const { return this->_on_screen; }
	void setVisible(const sf::Vector2f& view_center);
	void setInvisible() { this->_on_screen = false; }
	sf::FloatRect getGlobBound() const { return this->_unclicked.getGlobalBounds(); }

public:
	static constexpr char default_font_name[] = "Arial.ttf";
	static constexpr int button_colors[] = {60, 255, 20, 220};

	static constexpr float slot_x =  -(GameLogic::initial_window_width / 6.f);
	static constexpr float slot1_y = 0.f, slot2_y =  GameLogic::initial_window_height / 6.f;

private:
	sf::Font _font;
	sf::Text _title;
	sf::RectangleShape _unclicked;
	sf::RectangleShape _mouse_on;
	sf::RectangleShape _clicked;
	ButtonMode _mode;
	bool _on_screen;
	sf::Vector2f _position;
};

