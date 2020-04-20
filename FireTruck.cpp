#include "FireTruck.h"

sf::Texture FireTruck::texture;

bool FireTruck::ShouldSpawnFireFighter(float dt)
{
	if (onFire)
		return false;

	firefighterTimer += dt;

	if (sprite.getPosition().x < 140 && sprite.getPosition().x > 60 && sprite.getPosition().y < 100)
		return false;


	if (firefighterTimer > 5) {
		firefighterTimer = 0;
		return true;
	}
	return false;
}

FireTruck::FireTruck(float waitTime) : fireSystem(sf::Vector2f(0,0))
{
	deployTimer = waitTime;
	sprite.setTexture(texture);
	sprite.setOrigin(25, 13);

	fireSystem.on = false;
	fireSystem.scale = sf::Vector2f(0.75, 0.75);
	fireSystem.info.aliveTime = 2;
	fireSystem.info.minColorTime = 0.3;

	int random = (rand() % (4 - 1 + 1) + 1);

	if (random == 1) {
		sprite.setPosition(-50, 50);
		sprite.setTextureRect(sf::IntRect(50, 0, -50, 26));
		startLeft = true;
	}
	else if (random == 2) {
		sprite.setPosition(-50, 150);
		sprite.setTextureRect(sf::IntRect(50, 0, -50, 26));
		startLeft = true;
	}
	else if (random == 3) {
		sprite.setPosition(250, 50);
		startLeft = false;
	}
	else {
		sprite.setPosition(250, 150);
		startLeft = false;
	}
}


FireTruck::~FireTruck()
{
}

void FireTruck::Draw(sf::RenderWindow * window)
{
	window->draw(sprite);
	fireSystem.Draw(window);
}

void FireTruck::Update(float dt, sf::RenderWindow * window)
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
		sprite.setScale(1.2, 1.2);
	}
	else {
		sprite.setScale(1, 1);
	}

	sf::Vector2f dir = sf::Vector2f(100, 100) - sprite.getPosition();
	float dist = sqrt(dir.x*dir.x + dir.y*dir.y);
	if (dist < 0.01) dir = sf::Vector2f(0, 0); else dir /= dist;
	if (dist < 25)
		onFire = true;

	if (startLeft) {
		sprite.move(1 * 16 * dt, 0);

		if (sprite.getPosition().x > 220)
			readyToDie = true;
	}
	else {
		sprite.move(-1 * 16 * dt, 0);

		if (sprite.getPosition().x < -20)
			readyToDie = true;
	}

	if (onFire) {
		fireSystem.on = true;
	}

	fireSystem.position = sprite.getPosition();
	fireSystem.Update(dt);
}
