#include "WaterSystem.h"



WaterSystem::WaterSystem(sf::Vector2f position) : ParticleSystem(position)
{
	timeElapsed = 0;

	if (!particleTexture.loadFromFile("res/fire_circle.png")) {
		printf("Could not load fire texture...\n");
	}

	info.minColorTime = 1;
	info.aliveTime = 3;
	info.scale = sf::Vector2f(0.2, 0.2);
}


WaterSystem::~WaterSystem()
{
}

void WaterSystem::Draw(sf::RenderWindow * window)
{
	for (int i = 0; i < particles.size(); i++) {
		particles[i].Draw(window);
	}
}

void WaterSystem::Update(float dt)
{
	timeElapsed += dt;

	//loop every 1 second
	if (timeElapsed > 0.05 && on) {
		timeElapsed = 0;

		//determine random direction
		sf::Vector2f dir = sf::Vector2f(100, 100) - position;
		dir /= sqrt((position.x - 100)*(position.x - 100) + (position.y - 100)*(position.y - 100));

		//determine random direction
		float startX = dir.x;
		float startY = dir.y;
		int range = 10;

		int degrees = (rand() % (range - 0 + 1)) - range / 2;
		float rad = (float)degrees * PI / 180.0;

		//arctan(y/x) = theta +- random
		//x = cos(theta) y = sin(theta)
		float theta = atan2(startY, startX);
		theta += rad;
		float d_x = cos(theta);
		float d_y = sin(theta);

		//determine random speed
		int randomSpeed = 20 + (rand() % (10 - 0 + 1) + 0);


		info.direction = sf::Vector2f(d_x, d_y);
		info.speed = randomSpeed;
		info.startPosition = position;
		info.texture = &particleTexture;

		std::vector<sf::Color> colors;

		colors.push_back(sf::Color(20, 97, 250, 100));
		colors.push_back(sf::Color(94, 193, 242, 100));
		colors.push_back(sf::Color(255, 255, 255, 200));


		info.texture = &particleTexture;
		info.colors = colors;

		Particle particle = Particle(&info);
		//printf("%f\n", particle.info.scale);


		try {
			//printf("size before: %d", particles.size());
			particles.push_back(particle);
			//printf("  size after: %d\n", particles.size());


		}
		catch (const std::bad_alloc &) {
			printf("bad alloc\n");
		}
		catch (const std::exception &) {
			printf("exception!\n");
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].Update(dt);

		if (particles[i].elapsedTime > particles[i].info.aliveTime) {
			//printf("%f\n", particles[i].elapsedTime);
			particles.erase(particles.begin() + i);
		}
	}
}
