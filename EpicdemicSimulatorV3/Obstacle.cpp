#include "Obstacle.h"

void Obstacle::update(const sf::Time delta)
{
	sf::Vector2f new_pos = this->getPosition();
	new_pos += sf::Vector2f(this->_velocity.x * delta.asMilliseconds(), this->_velocity.y * delta.asMilliseconds());
	this->_sprite.setPosition(new_pos);
}
