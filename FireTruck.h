#pragma once
#include "SFML/Graphics.hpp"
#include "Draggable.h"
#include "FireSystem.h"

class FireTruck : Draggable
{
public:
	FireTruck(float waitTime = 0);
	~FireTruck();

	static sf::Texture texture;
	sf::Sprite sprite;

	bool startLeft = false;

	bool onFire = false;
	FireSystem fireSystem;

	bool readyToDie = false;

	float deployTimer = 0;
	bool canMove = false;

	float firefighterTimer = 0;
	bool ShouldSpawnFireFighter(float dt);
	void Draw(sf::RenderWindow * window);
	void Update(float dt, sf::RenderWindow * window);

};

