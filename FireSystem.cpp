#include "FireSystem.h"



FireSystem::FireSystem(sf::Vector2f position) : ParticleSystem::ParticleSystem(position)
{
	timeElapsed = 0;

	if (!particleTexture.loadFromFile("res/fire_circle.png")) {
		printf("Could not load fire texture...\n");
	}

	info.minColorTime = 2;
	info.aliveTime = 10;
	info.scale = sf::Vector2f(1, 1);

}


FireSystem::~FireSystem()
{
}

void FireSystem::Draw(sf::RenderWindow * window) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].Draw(window);
	}
	/*for (int i = particles.size() - 1; i > particles.size() / 2 + 1; i--) {
		particles[i].Draw(window);
	}*/
}

 void FireSystem::Update(float dt) {
	timeElapsed += dt;

	//loop every 1 second
	if (timeElapsed > 0.1 && on) {
		timeElapsed = 0;

		//determine random direction
		float startX = 0;
		float startY = -1;
		int range = 30;

		int degrees = (rand() % (range - 0 + 1)) - range / 2;
		float rad = (float)degrees * PI / 180.0;

		//arctan(y/x) = theta +- random
		//x = cos(theta) y = sin(theta)
		float theta = atan2(startY, startX);
		theta += rad;
		float d_x = cos(theta);
		float d_y = sin(theta);

		//determine random speed
		int randomSpeed = 10 + (rand() % (10 - 0 + 1) + 0);


		info.direction = sf::Vector2f(d_x, d_y);
		info.speed = randomSpeed;
		info.startPosition = position;
		info.texture = &particleTexture;
		info.scale = scale;

		std::vector<sf::Color> colors;

		colors.push_back(sf::Color(255, 232, 97, 255));
		colors.push_back(sf::Color(255, 173, 51, 100));
		colors.push_back(sf::Color(163, 38, 51, 100));

		colors.push_back(sf::Color(61, 38, 48, 100));
		colors.push_back(sf::Color(45, 45, 45, 100));
		colors.push_back(sf::Color(45, 45, 45, 0));

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
