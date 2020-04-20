#include "ParticleSystem.h"



ParticleSystem::ParticleSystem(sf::Vector2f position)
{
	this->position = position;
	this->scale = sf::Vector2f(1, 1);

	on = true;
}


ParticleSystem::~ParticleSystem()
{
}

