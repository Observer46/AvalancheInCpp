#include "GameLogic.h"

GameLogic::GameLogic(const sf::Vector2f& player_size, const sf::Color& player_color) :
	_obstacles(), _player(player_size, player_color, sf::Vector2f(GameLogic::initial_window_width/2, GameLogic::initial_window_height/2)), 
	_window_width{ initial_window_width }, _timer{}, _game_view(), _score{0}, _max_height{}, _difficulty{1.f}, _next_difficulty_tier{50},
	_lava(sf::Vector2f(GameLogic::initial_window_width, GameLogic::initial_window_height / 6), sf::Vector2f(0, GameLogic::initial_window_height)),
	_game_over{ false }, _max_objects_height{GameLogic::initial_player_height/2}
{
	srand(time(NULL));
	this->terrainSetup();
	this->centerCamera();
	this->scoreSetup();
}

void GameLogic::centerCamera()
{
	this->_game_view.setCenter(sf::Vector2f(GameLogic::initial_window_width / 2, GameLogic::initial_window_height / 2));
	this->_game_view.setSize(sf::Vector2f(GameLogic::initial_window_width, GameLogic::initial_window_height));
}

void GameLogic::terrainSetup()
{
	sf::Color terrain_color(0, 0, 0, 128);
	Obstacle terrain(sf::Vector2f(initial_window_width + 200, 100), terrain_color, sf::Vector2f(-100, initial_window_height - 100));
	terrain.stopFalling();
	this->_obstacles.push_back(terrain);
}

void GameLogic::cameraUp()
{
	sf::Vector2f player_position = this->_player.getPosition();
	float upper_threshold = this->_game_view.getCenter().y - GameLogic::initial_window_height / 6.f;
	if (player_position.y < upper_threshold) {				// If we are above 4/6 of height of window, we move camera upwards
		float delta = player_position.y - upper_threshold;
		this->_game_view.move(sf::Vector2f(0, delta));
	}
}

void GameLogic::cameraDown()
{
	sf::Vector2f player_position = this->_player.getPosition();
	float lower_threshold = this->_game_view.getCenter().y + GameLogic::initial_window_height / 6.f;
	if (player_position.y > lower_threshold) {				// If we are below 4/6 of window, we move camera downwards
		float delta = player_position.y - lower_threshold;
		this->_game_view.move(sf::Vector2f(0, delta));
	}
}

void GameLogic::update(const sf::Time delta, const sf::Vector2u& window_size)
{
	sf::Vector2f gravity(0, GameLogic::default_gravity);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->_player.goLeft(delta);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->_player.goRight(delta);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->_player.jump();
	
	this->collisionDetection(delta);

	this->_player.update(delta, window_size);
	this->updateObstacles(delta);

	this->_timer += delta.asSeconds();
	if (this->_timer > GameLogic::spawn_timeout / this->_difficulty) {
		this->spawnFallingObstacle();
		this->_timer -= GameLogic::spawn_timeout / this->_difficulty;
	}

	this->_player.gravityFall(gravity, delta);
	this->fallObstacles(gravity, delta);
	
	this->_player.slow(delta);

	this->cameraUp();
	this->cameraDown();

	this->increaseScore();
	this->increaseDifficulty();

	float speed_coefficient = 0.4f * this->_difficulty * this->_difficulty;
	this->_lava.increaseLavaLevel(delta, speed_coefficient);
}

void GameLogic::increaseScore()
{
	float player_altitude = this->_player.getPosition().y;
	float difference = this->_max_height - player_altitude;
	
	unsigned int score_diff = difference > 0 ? (unsigned) (difference /  10): 0;
	if (score_diff > 0) {
		this->_score += score_diff;
		this->_max_height -= difference;
	}
}

void GameLogic::increaseDifficulty()
{
	if (this->_score >= this->_next_difficulty_tier) {
		this->_next_difficulty_tier += GameLogic::difficulty_tier_modifier;
		this->_difficulty += GameLogic::max_difficulty - this->_difficulty / 6;
	}
}

void GameLogic::collisionDetection(const sf::Time& delta)
{
	this->checkPlayerLavaCollision();

	for (const auto& obstacle : this->_obstacles)
		if ( this->_player.checkPlayerCollision(obstacle, delta) &&  obstacle.isFalling())		// Checking if player died to falling obstacle
			this->death();
	
	for (size_t i = 1; i < this->_obstacles.size(); i++) {
		Obstacle& falling = this->_obstacles[i];
		if (falling.isFalling())
			for (size_t j = 0; j < this->_obstacles.size(); j++) 
				if (i != j) {
					Obstacle obstacle = this->_obstacles[j];
					this->checkFallingObstacleCollision(falling, obstacle, delta);
				}
	}
}

void GameLogic::checkFallingObstacleCollision(GameObject& falling, const GameObject& other, const sf::Time& delta)
{
	sf::FloatRect falling_shape = falling.nextMove(delta);
	sf::FloatRect obstacle_shape = other.getShape();

	//Intersection
	if (falling_shape.intersects(obstacle_shape)) {

		//Top side of obstacle
		if (falling_shape.top < obstacle_shape.top
			&& falling_shape.left + falling_shape.width > obstacle_shape.left
			&& falling_shape.left < obstacle_shape.left + obstacle_shape.width)
		{
			this->handleFallingObstacleCollision(falling_shape, obstacle_shape, CollisionType::Top, falling);
		}

		//Bottom side of obstacle
		else if (falling_shape.top + falling_shape.height > obstacle_shape.top + obstacle_shape.height
			&& falling_shape.left + falling_shape.width > obstacle_shape.left
			&& falling_shape.left < obstacle_shape.left + obstacle_shape.width)
		{
			this->handleFallingObstacleCollision(falling_shape, obstacle_shape, CollisionType::Bottom, falling);
		}
	}
}

void GameLogic::handleFallingObstacleCollision(const sf::FloatRect& falling_shape, const sf::FloatRect& obstacle_shape, const CollisionType& col_type, GameObject& falling)
{
	switch (col_type)
	{
	case CollisionType::Top:
		falling.instantVerticalStop();
		falling.stopFalling();
		if (falling.getPosition().y < this->_max_objects_height)
			this->_max_objects_height = falling.getPosition().y;	// Update height of "mountain"

		falling.setPosition(sf::Vector2f(falling_shape.left, obstacle_shape.top - falling_shape.height));	// Pop out of object if inside
		break;

	case CollisionType::Bottom:
		falling.instantVerticalStop();
		falling.setPosition(sf::Vector2f(falling_shape.left, obstacle_shape.top + obstacle_shape.height));	// Pop out of object if inside
		break;

	default:
		break;
	}
}

void GameLogic::spawnFallingObstacle()
{
	float obst_width = static_cast <float> (GameLogic::max_obstacle_size / 3 +  rand() / static_cast<float> (RAND_MAX / GameLogic::max_obstacle_size));
	float obst_height = static_cast <float> (GameLogic::max_obstacle_size / 3 + rand() / static_cast<float> (RAND_MAX / GameLogic::max_obstacle_size));
	float obst_x_pos = static_cast <float> (rand() / static_cast<float> (RAND_MAX / this->_window_width));
	sf::Color obst_color(rand() % 256, rand() % 256, rand() % 256, 255);
	Obstacle obst(sf::Vector2f(obst_width, obst_height), obst_color, sf::Vector2f(obst_x_pos, this->_max_objects_height - GameLogic::initial_window_height));
	this->_obstacles.push_back(obst);

}

void GameLogic::fallObstacles(const sf::Vector2f& gravity, const sf::Time& delta)
{
	for (auto& obst : this->_obstacles) {
		obst.gravityFall(gravity, delta);
	}
	
}

void GameLogic::updateObstacles(const sf::Time delta)
{
	for (auto& obst : this->_obstacles)
		obst.update(delta);
}

void GameLogic::death() {
	this->_player.deathAnimation();
	this->_game_over = true;
}

void GameLogic::checkPlayerLavaCollision() {
	sf::FloatRect player_shape = this->_player.getShape();
	sf::FloatRect lava_shape = this->_lava.getShape();

	if (player_shape.intersects(lava_shape))
		this->death();
}