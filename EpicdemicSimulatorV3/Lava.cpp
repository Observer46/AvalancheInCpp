#include "Lava.h"

Lava::Lava(const sf::Vector2f& initial_shape, const sf::Vector2f& initial_position) : GameObject(initial_shape, sf::Color::Red, initial_position)
{
	this->_falling = false;
	this->_sprite.setOutlineThickness(0.f);
	this->_sprite.setFillColor(sf::Color(255, 0, 0, 128));
}

void Lava::increaseLavaLevel(const sf::Time& delta, const float& coefficient)
{
	float actual_speed = Lava::lava_speed * coefficient;	// actual_speed < 0 
	float height_diff = actual_speed * delta.asMilliseconds();	// height_diff < 0
	sf::Vector2f new_pos = this->getPosition();
	sf::Vector2f new_size = this->_sprite.getSize();
	new_pos.y += height_diff;			// because height_diff < 0
	new_size.y -= height_diff;			// because height_diff < 0
	this->_sprite.setPosition(new_pos);
	this->_sprite.setSize(new_size);
}
