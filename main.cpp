#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FireSystem.h"
#include "FireFighter.h"
#include "FireTruck.h"
#include "Helicopter.h"

bool gameWin = false;
int currentWave = 1;
void wavestart(int wavenum);
void waveend();

//Lists of entities
std::vector<FireFighter> fighters;
std::vector<FireTruck> firetrucks;
std::vector<Helicopter> copters;

void init() {
	if (!FireFighter::texture.loadFromFile("res/firefighter.png")) {
		printf("Error loading firefighter texture...\n");
	}

	FireFighter::idleRight.setSpriteSheet(FireFighter::texture);
	FireFighter::idleRight.addFrame(sf::IntRect(0, 16, 16, 16));
	FireFighter::idleRight.addFrame(sf::IntRect(16, 16, 16, 16));
	FireFighter::idleRight.addFrame(sf::IntRect(32, 16, 16, 16));
	FireFighter::idleRight.addFrame(sf::IntRect(48, 16, 16, 16));

	FireFighter::idleLeft.setSpriteSheet(FireFighter::texture);
	FireFighter::idleLeft.addFrame(sf::IntRect(0, 0, 16, 16));
	FireFighter::idleLeft.addFrame(sf::IntRect(16, 0, 16, 16));
	FireFighter::idleLeft.addFrame(sf::IntRect(32, 0, 16, 16));
	FireFighter::idleLeft.addFrame(sf::IntRect(48, 0, 16, 16));

	if (!FireTruck::texture.loadFromFile("res/firetruck.png")) {
		printf("Error loading firetruck texture...\n");
	}

	if (!Helicopter::texture.loadFromFile("res/helicopter.png")) {
		printf("Error loading helicopter texture...\n");
	}

	Helicopter::right.setSpriteSheet(Helicopter::texture);
	Helicopter::right.addFrame(sf::IntRect(0, 20, 32, 20));
	Helicopter::right.addFrame(sf::IntRect(32, 20, 32, 20));
	Helicopter::right.addFrame(sf::IntRect(64, 20, 32, 20));
	Helicopter::right.addFrame(sf::IntRect(96, 20, 32, 20));

	Helicopter::left.setSpriteSheet(Helicopter::texture);
	Helicopter::left.addFrame(sf::IntRect(0, 0, 32, 20));
	Helicopter::left.addFrame(sf::IntRect(32, 0, 32, 20));
	Helicopter::left.addFrame(sf::IntRect(64, 0, 32, 20));
	Helicopter::left.addFrame(sf::IntRect(96, 0, 32, 20));

	if (!FireFighter::screamBuffer.loadFromFile("res/scream.wav")) {
		printf("Error loading scream sound...\n");
	}
	FireFighter::scream.setBuffer(FireFighter::screamBuffer);
	FireFighter::scream.setVolume(5);
}

int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 800), "LD46", sf::Style::Titlebar | sf::Style::Close);
	sf::View view;
	view.reset(sf::FloatRect(0, 0, 200, 200));

	FireSystem system = FireSystem(sf::Vector2f(100,100));

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("res/background.png")) {
		printf("Error loading background texture...\n");
	}
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);

	init();

	float health = 150;
	sf::RectangleShape healthBar;
	healthBar.setSize(sf::Vector2f(150, 10));
	healthBar.setFillColor(sf::Color(255,0,0,100));
	healthBar.setPosition(sf::Vector2f(25, 180));

	sf::Texture cursorTexture;
	if (!cursorTexture.loadFromFile("res/cursor.png")) {
		printf("Error loading cursor textuyre...\n");
	}

	sf::Sprite cursorSprite;
	cursorSprite.setTexture(cursorTexture);
	cursorSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	cursorSprite.setScale(0.4, 0.4);
	cursorSprite.setOrigin(3, 0);

	window.setMouseCursorVisible(false);

	sf::Texture deathScreenTexture;
	if (!deathScreenTexture.loadFromFile("res/deathscreen.png")) {
		printf("Error loading death dexture...\n");
	}
	sf::Sprite deathScreenSprite;
	deathScreenSprite.setTexture(deathScreenTexture);

	sf::Texture winScreenTexture;
	if (!winScreenTexture.loadFromFile("res/winscreen.png")) {
		printf("Error loading win texture...\n");
	}
	sf::Sprite winScreenSprite;
	winScreenSprite.setTexture(winScreenTexture);

	sf::SoundBuffer fireSoundBuffer;
	if (!fireSoundBuffer.loadFromFile("res/fire.wav")) {
		printf("Error loading fire sound...\n");
	}

	sf::Sound fireSound;
	fireSound.setBuffer(fireSoundBuffer);
	fireSound.setLoop(true);
	fireSound.play();

	float timeSinceLastDamage = 0;

	sf::Clock clock;

	wavestart(currentWave);

	//MAIN LOOP
	while (window.isOpen())
	{

		sf::Time elapsedTime = clock.getElapsedTime();
		clock.restart();
		float dt = elapsedTime.asSeconds();

		timeSinceLastDamage += dt;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//check for quit
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		//Update everything
		if (health > 0 && !gameWin) {


			float toScale = sqrt(health / 150);
			system.scale = sf::Vector2f(toScale, toScale);

			if(fireSound.getStatus() != sf::Sound::Playing)
				fireSound.play();
			fireSound.setVolume(toScale * 50);
			if (fireSound.getVolume() < 5)
				fireSound.setVolume(0);


			system.Update(dt);

			for (int i = 0; i < copters.size(); i++) {
				copters[i].Update(dt, &window);

				if (copters[i].isDamagingFire(dt)) {
					health -= 10;
					timeSinceLastDamage = 0;
				}
			}
			for (int i = 0; i < firetrucks.size(); i++) {
				firetrucks[i].Update(dt, &window);
				if (firetrucks[i].ShouldSpawnFireFighter(dt)) {
					FireFighter fighter;
					fighter.sprite.setPosition(firetrucks[i].sprite.getPosition());
					fighter.sprite.move(sf::Vector2f(0, 10));
					fighters.push_back(fighter);
				}
			}

			for (int i = 0; i < fighters.size(); i++) {
				fighters[i].Update(dt, &window);
				if (fighters[i].isDamagingFire(dt)) {
					health--;
					timeSinceLastDamage = 0;
				}
			}

			if(timeSinceLastDamage > 2)
				health += timeSinceLastDamage * dt;
			if (health > 150)
				health = 150;

		}
		
		//Draw everything

		window.setView(view);
		window.clear();

		window.draw(backgroundSprite);

		for (int i = 0; i < firetrucks.size(); i++) {
			firetrucks[i].Draw(&window);
		}

		system.Draw(&window);


		for (int i = 0; i < fighters.size(); i++) {
			fighters[i].Draw(&window);
		}

		for (int i = 0; i < copters.size(); i++) {
			copters[i].Draw(&window);
		}

		healthBar.setSize(sf::Vector2f(health, healthBar.getSize().y));
		window.draw(healthBar);

		//DEATH CONDITION
		if (health <= 0) {
			fireSound.stop();
			window.draw(deathScreenSprite);
			health = 0;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				health = 150;
				currentWave = 0;
				waveend();
			}
		}

		//GAME WIN
		if (gameWin) {
			fireSound.stop();
			window.draw(winScreenSprite);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				health = 150;
				gameWin = false;
				currentWave = 0;
				waveend();
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			cursorSprite.setTextureRect(sf::IntRect(16, 0, 16, 16));
		}
		else {
			cursorSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		mousePos /= (800 / 200);
		cursorSprite.setPosition(sf::Vector2f(mousePos.x, mousePos.y));

		if(!(mousePos.x < 0 || mousePos.y < 0 || mousePos.x > 200 || mousePos.y > 200))
			window.draw(cursorSprite);

		window.display();


		//check for wave end condition
		bool allDead = true;
		for (int i = 0; i < fighters.size(); i++) {
			if (!fighters[i].readyToDie)
				allDead = false;
		}
		for (int i = 0; i < firetrucks.size(); i++) {
			if (!firetrucks[i].readyToDie) //doesn't work yet
				allDead = false;
		}
		for (int i = 0; i < copters.size(); i++) {
			if (!copters[i].readyToDie)
				allDead = false;
		}

		if (allDead) {
			waveend();
		}
	}

	return 0;
}

void wavestart(int wavenum)
{
	//fighters.push_back(FireFighter());
	//firetrucks.push_back(FireTruck());
	//copters.push_back(Helicopter());
	if (wavenum == 10)
		printf("Final wave!\n");
	else if(wavenum < 10)
		printf("Wave %d...\n", wavenum);

	switch (wavenum) {
	case 1:
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		break;
	case 2:
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		firetrucks.push_back(FireTruck());
		break;
	case 3:
		copters.push_back(Helicopter());
		copters.push_back(Helicopter(3));
		copters.push_back(Helicopter(6));
		break;
	case 4:
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		firetrucks.push_back(FireTruck());
		firetrucks.push_back(FireTruck(5));
		fighters.push_back(FireFighter(7));
		fighters.push_back(FireFighter(7));
		copters.push_back(Helicopter(10));
		break;
	case 5:
		firetrucks.push_back(FireTruck());
		firetrucks.push_back(FireTruck(2));
		firetrucks.push_back(FireTruck(4));
		firetrucks.push_back(FireTruck(6));
		copters.push_back(Helicopter(3));
		copters.push_back(Helicopter(6));
		copters.push_back(Helicopter(9));
		break;
	case 6:
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		copters.push_back(Helicopter(3));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		copters.push_back(Helicopter(8));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		copters.push_back(Helicopter(14));
		break;
	case 7:
		firetrucks.push_back(FireTruck());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		copters.push_back(Helicopter(3));
		firetrucks.push_back(FireTruck(3));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		fighters.push_back(FireFighter(5));
		copters.push_back(Helicopter(8));
		firetrucks.push_back(FireTruck(8));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		fighters.push_back(FireFighter(10));
		firetrucks.push_back(FireTruck(14));
		copters.push_back(Helicopter(14));
		break;

	case 8:
		copters.push_back(Helicopter(0));
		copters.push_back(Helicopter(1));
		copters.push_back(Helicopter(2));
		copters.push_back(Helicopter(3));
		copters.push_back(Helicopter(4));
		copters.push_back(Helicopter(5));
		copters.push_back(Helicopter(6));
		copters.push_back(Helicopter(7));
		copters.push_back(Helicopter(8));
		break;
	case 9:
		for (int i = 0; i < 20; i += 4) {
			copters.push_back(Helicopter(i));
			fighters.push_back(FireFighter(i));
			fighters.push_back(FireFighter(i));
			fighters.push_back(FireFighter(i));
			firetrucks.push_back(FireTruck(i));
		}
		break;
	case 10:
		copters.push_back(Helicopter(0));
		copters.push_back(Helicopter(2));
		copters.push_back(Helicopter(4));
		copters.push_back(Helicopter(6));
		copters.push_back(Helicopter(8));
		firetrucks.push_back(FireTruck());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		fighters.push_back(FireFighter());
		firetrucks.push_back(FireTruck(6));
		fighters.push_back(FireFighter(6));
		fighters.push_back(FireFighter(6));
		fighters.push_back(FireFighter(6));
		fighters.push_back(FireFighter(6));
		fighters.push_back(FireFighter(6));
		fighters.push_back(FireFighter(6));

		firetrucks.push_back(FireTruck(14));
		firetrucks.push_back(FireTruck(14));
		firetrucks.push_back(FireTruck(14));
		firetrucks.push_back(FireTruck(14));
		break;
	default:
		gameWin = true;
	}

}

void waveend()
{
	fighters.clear();
	firetrucks.clear();
	copters.clear();
	wavestart(++currentWave);
}
