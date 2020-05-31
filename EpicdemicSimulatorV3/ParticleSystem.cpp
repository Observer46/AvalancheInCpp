#include "ParticleSystem.h"

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;
	target.draw(this->_vertices, states);
}

void ParticleSystem::update(sf::Time delta)
{
	for (size_t i = 0; i < this->_particles.size(); i++) {
		Particle& particle = this->_particles[i];
		particle._lifetime -= delta;
		if (particle._lifetime <= sf::Time::Zero)							// If time's up, we reset
			reset_particle(i);

		this->_vertices[i].position += particle._velocity * delta.asSeconds();		// Move particle
		float ratio = particle._lifetime.asSeconds() / this->_lifetime.asSeconds();
		this->_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
		this->_vertices[i].color.r = static_cast<sf::Uint8>(rand() % 255);
		this->_vertices[i].color.b = static_cast<sf::Uint8>(rand() % 255);
		this->_vertices[i].color.g = static_cast<sf::Uint8>(rand() % 255);

	}
}

void ParticleSystem::reset_particle(const size_t& i)
{
	float angle = (rand() % 360) * 3.14f / 180.f;
	float speed = (rand() % 50) + 50.f;
	this->_particles[i]._velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
	this->_particles[i]._lifetime = sf::milliseconds((this->_lifetime.asMilliseconds() - 2 * this->_lifetime.asMilliseconds() / 3)  + (rand() % (this->_lifetime.asMilliseconds() - this->_lifetime.asMilliseconds() / 3) ));
	this->_vertices[i].position = this->_emitter;
}
