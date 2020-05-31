#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Lava : public GameObject
{
public:
	Lava(const sf::Vector2f& initial_shape, const sf::Vector2f& initial_position);
	void increaseLavaLevel(const sf::Time& delta, const float& coefficient);

public:
	static constexpr float lava_speed = -0.01f ;
};

