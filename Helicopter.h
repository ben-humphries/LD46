#pragma once
#include "SFML/Graphics.hpp"
#include "AnimatedSprite.h"
#include "WaterSystem.h"
#include "Draggable.h"
#include "FireSystem.h"

class Helicopter : Draggable
{
public:
	Helicopter(float waitTime = 0);
	~Helicopter();

	static Animation right;
	static Animation left;

	bool startLeft;

	bool onFire = false;

	bool readyToDie = false;

	static sf::Texture texture;
	AnimatedSprite sprite;

	WaterSystem waterSystem;
	FireSystem fireSystem;

	float damageTimer = 0;
	bool isDamagingFire(float dt);

	float deployTimer = 0;
	bool canMove = false;

	void Draw(sf::RenderWindow * window);
	void Update(float dt, sf::RenderWindow * window);
};

