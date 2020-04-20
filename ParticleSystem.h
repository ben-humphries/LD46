#pragma once
#include "Particle.h"
#include <math.h>

#define PI 3.14159265358979323846264338327950288

class ParticleSystem
{
public:
	ParticleSystem(sf::Vector2f position);
	~ParticleSystem();

	float timeElapsed = 0;
	std::vector<Particle> particles;
	sf::Texture particleTexture;
	sf::Vector2f position;
	sf::Vector2f scale;

	bool on;

};

