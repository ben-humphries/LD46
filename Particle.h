#pragma once
#include "SFML/Graphics.hpp"

struct ParticleInfo {
	sf::Texture * texture;
	sf::Vector2f startPosition;
	sf::Vector2f direction;
	std::vector<sf::Color> colors; //min size 2
	float minColorTime;
	float aliveTime;
	float speed;
	sf::Vector2f scale;
};


class Particle
{
public:
	Particle(ParticleInfo * particleInfo);
	~Particle();

	ParticleInfo info;

	sf::Sprite sprite;

	int colorIndex = 1;
	float colorTimer;
	float elapsedTime;

	float currentColorR;
	float currentColorG;
	float currentColorB;
	float currentColorA;

	void Draw(sf::RenderWindow * window);
	void Update(float dt);
};

