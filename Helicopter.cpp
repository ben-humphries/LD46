#include "Helicopter.h"

sf::Texture Helicopter::texture;
Animation Helicopter::right;
Animation Helicopter::left;

bool Helicopter::isDamagingFire(float dt)
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

Helicopter::Helicopter(float waitTime) : waterSystem(sf::Vector2f(0, 0)), fireSystem(sf::Vector2f(0, 0))
{

	deployTimer = waitTime;
	sprite = AnimatedSprite(sf::seconds(0.05), false, true);
	sprite.setScale(1.4, 1.4);
	sprite.setOrigin(16, 10);

	srand(time(NULL));
	if (((rand() %(1 - 0 + 1)) + 0) == 0)
		startLeft = true;
	else
		startLeft = false;

	if (startLeft)
		sprite.setPosition(-20, 16);
	else
		sprite.setPosition(220, 16);
	
	fireSystem.scale = sf::Vector2f(0.5, 0.5);
	fireSystem.info.aliveTime = 2;
	fireSystem.info.minColorTime = 0.3;

	waterSystem.info.scale = sf::Vector2f(0.5, 0.5);
}


Helicopter::~Helicopter()
{
}

void Helicopter::Draw(sf::RenderWindow * window)
{
	waterSystem.Draw(window);
	window->draw(sprite);
	fireSystem.Draw(window);
}

void Helicopter::Update(float dt, sf::RenderWindow * window)
{
	if (deployTimer > 0) {
		deployTimer -= dt;
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (thisFirefighterPickedUp || !Draggable::firefighterPickedUp)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		mousePos /= (800 / 200);
		sf::Vector2f spritePos = sprite.getPosition();
		float dist = sqrt((spritePos.x - mousePos.x)*(spritePos.x - mousePos.x) + (spritePos.y - mousePos.y)*(spritePos.y - mousePos.y));
		if (dist < 15) {
			sprite.setPosition(sf::Vector2f(mousePos.x, mousePos.y));
			Draggable::firefighterPickedUp = true;
			thisFirefighterPickedUp = true;
		}
		else {
			Draggable::firefighterPickedUp = false;
			thisFirefighterPickedUp = false;
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		Draggable::firefighterPickedUp = false;
		thisFirefighterPickedUp = false;
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	mousePos /= (800 / 200);
	sf::Vector2f spritePos = sprite.getPosition();
	float distance = sqrt((spritePos.x - mousePos.x)*(spritePos.x - mousePos.x) + (spritePos.y - mousePos.y)*(spritePos.y - mousePos.y));
	if (distance < 15) {
		sprite.setScale(1.6, 1.6);
	}
	else {
		sprite.setScale(1.4, 1.4);
	}

	if (startLeft) {
		sprite.move(1 * 120 * dt, 0);
		sprite.play(right);

		if (sprite.getPosition().x > 300) {
			startLeft = false;
			sprite.move(0, 40);
		}
	}
	else {
		sprite.move(-1 * 120 * dt, 0);
		sprite.play(left);

		if (sprite.getPosition().x < -100) {
			startLeft = true;
			sprite.move(0, 40);
		}
	}

	sf::Vector2f dir = sf::Vector2f(100, 100) - sprite.getPosition();
	float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
	if (dist < 0.01) dir = sf::Vector2f(0, 0); else dir /= dist;

	if (dist < 25 && thisFirefighterPickedUp)
		onFire = true;

	if (sprite.getPosition().y > 250)
		readyToDie = true;

	if (abs(100 - sprite.getPosition().x) < 75 && sprite.getPosition().y < 200) {
		waterSystem.on = true;
	}
	else
		waterSystem.on = false;

	if (Draggable::thisFirefighterPickedUp || onFire)
		waterSystem.on = false;
	if (onFire)
		fireSystem.on = true;
	else
		fireSystem.on = false;

	sprite.update(sf::seconds(dt));
	waterSystem.position = sprite.getPosition() + sf::Vector2f(0, 15);
	waterSystem.Update(dt);
	fireSystem.position = sprite.getPosition();
	fireSystem.Update(dt);
}
