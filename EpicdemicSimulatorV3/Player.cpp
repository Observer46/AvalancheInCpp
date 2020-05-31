#include "Player.h"
#include "GameLogic.h"

void Player::checkWindowCollision(const sf::Vector2u& window_size)
{
	sf::FloatRect player_shape = this->_sprite.getGlobalBounds();
	if (player_shape.left >= 0 && player_shape.left + player_shape.width < window_size.x)
		this->_outside_window = false;
	else {
		this->_outside_window = true;
		sf::Vector2f clone_position(player_shape.left - window_size.x, player_shape.top);
		this->_clone.setPosition(clone_position);
	}
}

void Player::update(const sf::Time& delta, const sf::Vector2u& window_size)
{
	this->checkWindowCollision(window_size);
	this->move(delta);
	this->resetPosIfOutsideWindow(window_size);
}

void Player::jump()
{
	if (!this->_jump) {
		this->_jump = true;
		this->_airborne = true;
		this->_velocity.y = Player::jump_value;
		if (this->_slide == SlideType::Left)
			this->_velocity.x = Player::off_wall_jump_speed;
		else if (this->_slide == SlideType::Right)
			this->_velocity.x = -Player::off_wall_jump_speed;
		this->_slide = SlideType::None;
		this->_falling = true;
	}
}

void Player::slide(const SlideType& slide_side)
{
	this->_slide = slide_side;
	this->_jump = false;	// Can jump away from the wall
	if (this->_airborne) {
		this->_falling = false;
		this->_velocity.y = 0.125f / 4;
		this->_velocity.x = 0.f;
		return;
	}
	this->_falling = true;
}

void Player::move(const sf::Time& delta)
{
	sf::Vector2f new_pos = this->getPosition();
	new_pos += sf::Vector2f(this->_velocity.x * delta.asMilliseconds(), this->_velocity.y * delta.asMilliseconds());
	this->_sprite.setPosition(new_pos);
}

void Player::goLeft(const sf::Time& delta)
{
	if (this->_slide == SlideType::Right) {
		this->_falling = true;
		this->_slide = SlideType::None;
	}
	this->_velocity.x -= Player::movement_speed * delta.asMilliseconds(); 
}

void Player::goRight(const sf::Time& delta)
{
	if (this->_slide == SlideType::Left) {
		this->_falling = true;
		this->_slide = SlideType::None;
	}
	this->_velocity.x += Player::movement_speed * delta.asMilliseconds();
}

void Player::slow(const sf::Time& delta)
{
	if (this->_velocity.x > 0) {
		this->_velocity.x -= 0.0625f * delta.asMilliseconds();
		if (this->_velocity.x <= 0)
			this->_velocity.x = 0;
	}
	else if (this->_velocity.x < 0) {
		this->_velocity.x += 0.0625f * delta.asMilliseconds();
		if (this->_velocity.x >= 0)
			this->_velocity.x = 0;
	}
}

sf::FloatRect Player::cloneNextMove(const sf::Time& delta) const
{
	sf::FloatRect shape = this->_clone.getGlobalBounds();
	sf::Vector2f size(shape.width, shape.height);
	sf::Vector2f newPos = this->cloneNextPosition(delta);
	return sf::FloatRect(newPos, size);
}

bool Player::checkIntersection(const sf::FloatRect& player_shape, const sf::FloatRect& obstacle_shape)
{
	bool return_value = false;
	//Bottom side of obstacle
	if (player_shape.top + player_shape.height > obstacle_shape.top + obstacle_shape.height
		&& player_shape.left + player_shape.width > obstacle_shape.left
		&& player_shape.left < obstacle_shape.left + obstacle_shape.width)
	{
		return_value = this->handlePlayerCollsion(player_shape, obstacle_shape, CollisionType::Bottom);
	}

	//Top side of obstacle
	if (player_shape.top < obstacle_shape.top
		&& player_shape.left + player_shape.width > obstacle_shape.left
		&& player_shape.left < obstacle_shape.left + obstacle_shape.width)
	{
		return_value = this->handlePlayerCollsion(player_shape, obstacle_shape, CollisionType::Top);
	}

	//Right side of obstacle
		if (player_shape.left < obstacle_shape.left
		&& player_shape.top + player_shape.height > obstacle_shape.top
		&& player_shape.top < obstacle_shape.top + obstacle_shape.height)
	{
		return_value = this->handlePlayerCollsion(player_shape, obstacle_shape, CollisionType::Right);
		if (this->_slide == SlideType::None)
			this->slide(SlideType::Right);
	}

	//Left side of obstacle
	else if (player_shape.left + player_shape.width > obstacle_shape.left + obstacle_shape.width
		&& player_shape.top + player_shape.height > obstacle_shape.top
		&& player_shape.top < obstacle_shape.top + obstacle_shape.height)
	{
		return_value = this->handlePlayerCollsion(player_shape, obstacle_shape, CollisionType::Left);
		if (this->_slide == SlideType::None)
			this->slide(SlideType::Left);
	}

	return return_value;
}

sf::Vector2f Player::cloneNextPosition(const sf::Time& delta) const
{
	sf::Vector2f new_pos = this->_clone.getPosition();
	new_pos += sf::Vector2f(this->_velocity.x * delta.asMilliseconds(), this->_velocity.y * delta.asMilliseconds());
	return new_pos;
}

void Player::resetPosIfOutsideWindow(const sf::Vector2u& window_size)
{
	sf::Vector2f position = this->getPosition();
	if (position.x >= (float) window_size.x)
		this->setPosition(sf::Vector2f(0, position.y));
	else if(position.x < 0)
		this->setPosition(sf::Vector2f((float) window_size.x, position.y));
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	GameObject::draw(target, states);
	if (this->_outside_window)
		target.draw(this->_clone, states);
}

bool Player::checkPlayerCollision(const GameObject& other, const sf::Time& delta)
{
	bool val1 = false;
	bool val2 = false;

	sf::FloatRect player_shape = this->nextMove(delta);
	sf::FloatRect obstacle_shape = other.getShape();
	if (player_shape.intersects(obstacle_shape))
		val1 = this->checkIntersection(this->_sprite.getGlobalBounds(), obstacle_shape);

	if (this->_outside_window) {
		player_shape = this->cloneNextMove(delta);
		if (player_shape.intersects(obstacle_shape))
			val2 = this->checkIntersection(this->_clone.getGlobalBounds(), obstacle_shape);
	}
	return val1 || val2;
}

bool Player::handlePlayerCollsion(const sf::FloatRect& player_shape, const sf::FloatRect& obstacle_shape, const CollisionType& col_type)
{
	switch (col_type)
	{
	case CollisionType::Left:
		this->instantHorizontalStop();
		return false;

	case CollisionType::Right:
		this->instantHorizontalStop();
		return false;

	case CollisionType::Top:
		this->instantVerticalStop();
		this->_airborne = false;
		this->canJumpNow();
		return false;

	case CollisionType::Bottom:
		this->instantVerticalStop();
		if (this->deathCheck())
			return true;

		return false;

	default:
		return false;
	}
}
