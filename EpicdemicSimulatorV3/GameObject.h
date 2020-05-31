#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


class GameObject : public sf::Drawable, public sf::Transformable {
public:
	GameObject(const sf::Vector2f& object_shape, const sf::Color& color, const sf::Vector2f& position = sf::Vector2f());

	sf::Vector2f getPosition() const { return this->_sprite.getPosition(); }
	sf::Vector2f getSize() const { return this->_sprite.getSize(); }
	sf::FloatRect getShape() const { return this->_sprite.getGlobalBounds(); }

	void setPosition(const sf::Vector2f& new_position) { this->_sprite.setPosition(new_position); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void instantVerticalStop() { this->_velocity.y = 0; }

	void stopFalling() { this->_falling = false; }
	void gravityFall(const sf::Vector2f& gravity, const sf::Time& delta);
	bool isFalling() const { return this->_falling; }

	sf::FloatRect nextMove(const sf::Time& delta) const;
	sf::Vector2f newPosition(const sf::Time& delta) const;

protected:
	static constexpr float max_gravity_value = 5.f;
	sf::RectangleShape _sprite;			// Because I don't want to play with textures :P
	sf::Vector2f _velocity;
	bool _falling;
};

