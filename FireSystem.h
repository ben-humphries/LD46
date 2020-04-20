#pragma once
#include "ParticleSystem.h"
class FireSystem :
	public ParticleSystem
{
public:
	FireSystem(sf::Vector2f position);
	~FireSystem();

	void Draw(sf::RenderWindow * window);
	void Update(float dt);
	ParticleInfo info;

};

