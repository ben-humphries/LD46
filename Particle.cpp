#include "Particle.h"

#define COLOR_CHANGE_CONSTANT 3.f

Particle::Particle(ParticleInfo * particleInfo)
{
	info = *particleInfo;
	currentColorR = (float) info.colors[0].r / 255;
	currentColorG = (float) info.colors[0].g / 255;
	currentColorB = (float) info.colors[0].b / 255;
	currentColorA = (float) info.colors[0].a / 255;

	sprite.setTexture(*info.texture);
	sprite.setPosition(info.startPosition);
	sf::Vector2u spriteSize = sprite.getTexture()->getSize();
	sprite.setOrigin(spriteSize.x / 2, spriteSize.y / 2);
	sprite.setColor(sf::Color(currentColorR * 255, currentColorG * 255, currentColorB * 255, currentColorA * 255));
	sprite.setScale(info.scale);

	elapsedTime = 0;
}


Particle::~Particle()
{
}

void Particle::Draw(sf::RenderWindow * window)
{
	window->draw(sprite);
}

void Particle::Update(float dt)
{
	elapsedTime += dt;

	sprite.move(info.direction * info.speed * dt);

	//update color
	colorTimer += dt;

	currentColorR += ((float) info.colors[colorIndex].r / 255 - currentColorR) * dt * COLOR_CHANGE_CONSTANT;
	currentColorG += ((float) info.colors[colorIndex].g / 255 - currentColorG) * dt * COLOR_CHANGE_CONSTANT;
	currentColorB += ((float) info.colors[colorIndex].b / 255 - currentColorB) * dt * COLOR_CHANGE_CONSTANT;
	currentColorA += ((float) info.colors[colorIndex].a / 255 - currentColorA) * dt * COLOR_CHANGE_CONSTANT;
	
	if ((float)info.colors[colorIndex].r / 255 - currentColorR < 0.1 &&
		((float)info.colors[colorIndex].g / 255 - currentColorG) < 0.1 &&
		((float)info.colors[colorIndex].b / 255 - currentColorB) < 0.1 &&
		((float)info.colors[colorIndex].a / 255 - currentColorA) < 0.1) {

		if (colorTimer > info.minColorTime) {
			colorTimer = 0;
			if (info.colors.size() - 1 > colorIndex) {
				colorIndex++;
			}
		}
	}

	sprite.setColor(sf::Color(currentColorR * 255, currentColorG * 255, currentColorB * 255, currentColorA * 255));
}
