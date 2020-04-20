#include "FireFighter.h"

sf::Texture FireFighter::texture;
 Animation FireFighter::idleRight;
 Animation FireFighter::idleLeft;
 bool FireFighter::firefighterPickedUp;

 sf::SoundBuffer FireFighter::screamBuffer;
 sf::Sound FireFighter::scream;

 bool FireFighter::isDamagingFire(float dt)
 {
	 if (deployTimer > 0)
		 return false;

	 if (waterSystem.on)
		 damageTimer += dt;

	 if (damageTimer > 1) {
		 damageTimer = 0;
		 return true;
	 }
	 return false;
 }

 FireFighter::FireFighter(float waitTime) : fireSystem(sf::Vector2f(0, 0)), waterSystem(sf::Vector2f(0,0))
{
	sprite = AnimatedSprite(sf::seconds(0.15), false, true);

	sf::Vector2f startPos;
	bool badPos = true;
	while (badPos) {
		//choose random starting position on circle of radius r
		float r = 140;
		float angle = 0 + (rand() % (360 - 0 + 1) + 0);
		startPos.x = r * cos(angle);
		startPos.y = r * sin(angle);
		startPos.x += 100;
		startPos.y += 100;

		badPos = startPos.x < 180 && startPos.x > 20 && startPos.y < 100;
	}

	sprite.setPosition(startPos);
	sprite.setOrigin(8, 8);

	fireSystem.position = sprite.getPosition();
	fireSystem.scale = sf::Vector2f(0.25, 0.25);
	fireSystem.info.aliveTime = 2;
	fireSystem.info.minColorTime = 0.3;

	waterSystem.position = sprite.getPosition();
	waterSystem.on = false;

	this->deployTimer = waitTime;
}


FireFighter::~FireFighter()
{
}

void FireFighter::Draw(sf::RenderWindow * window)
{
	waterSystem.Draw(window);
	window->draw(sprite);
	fireSystem.Draw(window);
}

void FireFighter::Update(float dt, sf::RenderWindow * window)
{
	if (deployTimer > 0) {
		deployTimer -= dt;
		return;
	}
	waterSystem.on = true;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (thisFirefighterPickedUp || !FireFighter::firefighterPickedUp)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		mousePos /= (800 / 200);
		sf::Vector2f spritePos = sprite.getPosition();
		float dist = sqrt((spritePos.x - mousePos.x)*(spritePos.x - mousePos.x) + (spritePos.y - mousePos.y)*(spritePos.y - mousePos.y));
		if (dist < 8) {
			sprite.setPosition(sf::Vector2f(mousePos.x, mousePos.y));
			FireFighter::firefighterPickedUp = true;
			thisFirefighterPickedUp = true;
		}
		else {
			FireFighter::firefighterPickedUp = false;
			thisFirefighterPickedUp = false;
		}
	}
	else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		FireFighter::firefighterPickedUp = false;
		thisFirefighterPickedUp = false;
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	mousePos /= (800 / 200);
	sf::Vector2f spritePos = sprite.getPosition();
	float distance = sqrt((spritePos.x - mousePos.x)*(spritePos.x - mousePos.x) + (spritePos.y - mousePos.y)*(spritePos.y - mousePos.y));
	if (distance < 8) {
		sprite.setScale(1.2, 1.2);
	}
	else {
		sprite.setScale(1, 1);
	}

	if (sprite.getPosition().x < 100) {
		if (onFire) {
			sprite.play(idleLeft);
		}
		else {
			sprite.play(idleRight);
		}
	}
	else {
		if (onFire) {
			sprite.play(idleRight);
		}
		else {
			sprite.play(idleLeft);
		}
	}
	sprite.update(sf::seconds(dt));

	sf::Vector2f dir = sf::Vector2f(100, 100) - sprite.getPosition();
	float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
	if (dist < 0.01) dir = sf::Vector2f(0, 0); else dir /= dist;

	if (dist > 75) {

		if (onFire) {
			dir = sprite.getPosition() - sf::Vector2f(100, 100);
			float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
			if (dist < 0.01) dir = sf::Vector2f(0, 0); else dir /= dist;

			sprite.play();
			sprite.move(dir.x * 40 * dt, dir.y * 40 * dt);
			waterSystem.on = false;
		}
		else {
			sprite.play();
			sprite.move(dir.x * 5 * dt, dir.y * 5 * dt);
			waterSystem.on = false;
		}
	}
	else {
		if (onFire) {
			dir = sprite.getPosition() - sf::Vector2f(100, 100);
			float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
			if (dist < 0.01) dir = sf::Vector2f(0, 0); else dir /= dist;

			sprite.play();
			sprite.move(dir.x * 40 * dt, dir.y * 40 * dt);
			waterSystem.on = false;
		}
		if(!thisFirefighterPickedUp)
			sprite.stop();
	}
	if (dist < 25) {
		if (!onFire)
			scream.play();
		onFire = true;
	}
	if (dist > 150)
		readyToDie = true;

	fireSystem.position = sprite.getPosition();
	if (onFire) {
		fireSystem.Update(dt);
		waterSystem.on = false;
	}

	if (thisFirefighterPickedUp) {
		waterSystem.on = false;
	}

	waterSystem.position = sprite.getPosition();
	waterSystem.Update(dt);

}
