#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
private:
	struct Particle
	{
		sf::Vector2f _velocity;
		sf::Time _lifetime;
	};

public:
	ParticleSystem(const unsigned int& count, const float& seconds) : 
		_vertices(sf::Points, count), 
		_lifetime(sf::seconds(seconds)), 
		_emitter(0.f, 0.f), 
		_particles(count)
	{};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void set_emitter(sf::Vector2f position) { this->_emitter = position; }
	void update(sf::Time delta);
	void reset_particle(const size_t& index);

private:
	sf::VertexArray _vertices;
	sf::Time _lifetime;
	sf::Vector2f _emitter;
	std::vector<Particle> _particles;
};

