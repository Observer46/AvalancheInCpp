#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameObject.h"
#include "CollisionType.h"

enum class SlideType {
	Left,
	Right,
	None
};


class Player : public GameObject
{
public:
	Player(const sf::Vector2f& initial_shape, const sf::Color& color, const sf::Vector2f& initial_pos) :
		GameObject(initial_shape, color, initial_pos), _clone(this->_sprite),
		_moving{ false }, _slide{ SlideType::None }, _outside_window{ false }, _jump{ false }, _airborne{ true } {};

	void checkWindowCollision(const sf::Vector2u& window_size);
	void update(const sf::Time& delta, const sf::Vector2u& window_size);
	void jump();
	void slide(const SlideType& slide_side);
	void startFalling() { this->_falling = true; }
	void move(const sf::Time& delta);
	void goLeft(const sf::Time& delta);
	void goRight(const sf::Time& delta);
	void slow(const sf::Time& delta);
	void instantHorizontalStop() { this->_velocity.x = 0; }
	void canJumpNow() { this->_jump = false; }
	sf::Vector2f cloneNextPosition(const sf::Time& delta) const;
	sf::FloatRect cloneNextMove(const sf::Time& delta) const;
	bool checkIntersection(const sf::FloatRect& player_shape, const sf::FloatRect& obstacle_shape);
	void resetPosIfOutsideWindow(const sf::Vector2u& window_size);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool checkPlayerCollision(const GameObject& other, const sf::Time& delta);
	bool handlePlayerCollsion(const sf::FloatRect& player_shape, const sf::FloatRect& obstacle_shape, const CollisionType& col_type);
	bool deathCheck() const { return !this->_jump; }
	void deathAnimation() { this->_sprite.setFillColor(sf::Color::Black); }		// Improve

private:
	static constexpr float jump_value = -0.75f;
	static constexpr float movement_speed = 0.02f;
	static constexpr float off_wall_jump_speed = 2.5f;
	sf::RectangleShape _clone;
	bool _moving;
	SlideType _slide;
	bool _outside_window;
	bool _jump;
	bool _airborne;
};

