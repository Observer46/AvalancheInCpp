#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "CollisionType.h"
#include "Obstacle.h"
#include "Player.h"
#include "Lava.h"

class GameLogic
{
public:
	GameLogic(const sf::Vector2f& player_size = sf::Vector2f(initial_player_width, initial_player_height), const sf::Color& player_color = sf::Color::White);

	Player getPlayer() const { return this->_player; }
	sf::View getView() const { return this->_game_view; }
	Lava getLava() const { return this->_lava; }
	unsigned int getScore() const { return this->_score; }

	void centerCamera();
	void terrainSetup();
	void scoreSetup() { this->_max_height = this->_player.getPosition().y; }

	void cameraUp();
	void cameraDown();
	void update(const sf::Time delta, const sf::Vector2u& window_size);
	std::vector<Obstacle> getObstacles() const { return this->_obstacles; }

	void increaseScore();
	void increaseDifficulty();
	bool isPlayerAlive() const { return !this->_game_over; }
	void death();

	void collisionDetection(const sf::Time& delta);
	void checkFallingObstacleCollision(GameObject& falling, const GameObject& other, const sf::Time& delta);
	void handleFallingObstacleCollision(const sf::FloatRect& falling_shape, const sf::FloatRect& obstacle_shape, const CollisionType& col_type, GameObject& falling);

	void spawnFallingObstacle();
	void fallObstacles(const sf::Vector2f& gravity, const sf::Time& delta);
	void updateObstacles(const sf::Time delta);
	void checkPlayerLavaCollision();

public:
	static constexpr unsigned initial_window_width = 800, initial_window_height = 600;
	static constexpr float initial_player_width = 30, initial_player_height = 50;
	static constexpr float default_gravity = 0.015625f / 8;
	static constexpr float max_obstacle_size = 150.f;
	static constexpr float max_gravity_value = 1.25f;
	static constexpr float spawn_timeout = 2.f;
	static constexpr unsigned difficulty_tier_modifier = 50;
	static constexpr float max_difficulty = 1.25f;

private:
	std::vector<Obstacle> _obstacles;
	Player _player;
	float _window_width;
	float _timer;
	sf::View _game_view;
	unsigned int  _score;
	float _max_height;
	float _difficulty;
	unsigned int _next_difficulty_tier;
	Lava _lava;
	bool _game_over;
	float _max_objects_height;
};

