#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameLogic.h"

class Title : public sf::Drawable, public sf::Transformable
{
public:
	Title();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(const sf::Time& delta, const sf::Vector2f view_center);
	void updateColor(sf::Text& letter, int color_change);
	void increaseColor(const sf::Color& prev_color, sf::Color& updated_color, int& color_change) const;
	void decreaseColor(const sf::Color& prev_color, sf::Color& updated_color, int& color_change) const;
	bool checkColorChangeDirection(const sf::Text& letter) const;

public:
	static constexpr float rotation_speed = 7.5f;
	static constexpr float max_rotation = 15.f;
	static constexpr float color_change_speed = 255.f;

private:
	sf::Font _title_font;
	//std::vector<sf::Text> _title_letters;
	sf::Text _title_letters;
	bool _rotate_cw;
};

