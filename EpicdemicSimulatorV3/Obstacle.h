#pragma once
#include <SFML/Graphics.hpp>

#include "GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle(const sf::Vector2f& initial_shape, const sf::Color& color, const sf::Vector2f& position) :
		GameObject(initial_shape, color, position) {};

	void update(const sf::Time delta);
};

