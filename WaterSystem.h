#pragma once
#include "ParticleSystem.h"

class WaterSystem :
	public ParticleSystem
{
public:
	WaterSystem(sf::Vector2f position);
	~WaterSystem();

	void Draw(sf::RenderWindow * window);
	void Update(float dt);
	ParticleInfo info;
};

