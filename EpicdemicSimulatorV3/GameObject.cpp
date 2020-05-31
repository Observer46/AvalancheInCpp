#include "GameObject.h"

GameObject::GameObject(const sf::Vector2f& object_shape, const sf::Color& color, const sf::Vector2f& position) :
	_sprite(object_shape), _velocity(), _falling{ true }
{
	this->_sprite.setFillColor(color);
	this->_sprite.setOutlineColor(sf::Color::Black);
	this->_sprite.setOutlineThickness(2.f);
	this->_sprite.setPosition(position);
}

void GameObject::gravityFall(const sf::Vector2f& gravity, const sf::Time& delta) {
	if (this->_falling)
		if (this->_velocity.y < GameObject::max_gravity_value)
			this->_velocity += sf::Vector2f(gravity.x * delta.asMilliseconds(), gravity.y * delta.asMilliseconds());
		else
			this->_velocity.y = 0;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = NULL;
	target.draw(this->_sprite, states);
}

sf::FloatRect GameObject::nextMove(const sf::Time& delta) const
{
	sf::FloatRect shape = this->getShape();
	sf::Vector2f size(shape.width, shape.height);
	sf::Vector2f newPos = this->newPosition(delta);
	return sf::FloatRect(newPos, size);
}

sf::Vector2f GameObject::newPosition(const sf::Time& delta) const
{
	sf::Vector2f new_pos = this->getPosition();
	new_pos += sf::Vector2f(this->_velocity.x * delta.asMilliseconds(), this->_velocity.y * delta.asMilliseconds());
	return new_pos;
}