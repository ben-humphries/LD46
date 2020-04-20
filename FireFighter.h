#pragma once
#include "SFML/Graphics.hpp"
#include "AnimatedSprite.h"
#include "FireSystem.h"
#include "WaterSystem.h"
#include "Draggable.h"
#include <SFML/Audio.hpp>

class FireFighter : Draggable
{
public:
	static sf::Texture texture;
	static Animation idleRight;
	static Animation idleLeft;

	static sf::SoundBuffer screamBuffer;
	static sf::Sound scream;

	//static bool firefighterPickedUp;
	//bool thisFirefighterPickedUp;

	AnimatedSprite sprite;
	FireSystem fireSystem;
	bool onFire = false;
	WaterSystem waterSystem;

	bool readyToDie = false;

	void Draw(sf::RenderWindow * window);
	void Update(float dt, sf::RenderWindow * window);

	float deployTimer = 0;
	bool canMove = false;

	float damageTimer = 0;
	bool isDamagingFire(float dt);
	FireFighter(float waitTime = 0);
	~FireFighter();
};

