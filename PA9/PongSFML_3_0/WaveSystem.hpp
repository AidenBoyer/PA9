#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"

class WaveSystem {

public:
	WaveSystem(const sf::Texture& baiscTexture)
		:basicEnemyTexture(baiscTexture), basicEnemy(basicEnemyTexture),
		currentWave(-1) {

	}
	const std::vector<Enemy>& getEnemies() const {
		return enemies;
	}
	void spawnWave(int waveNum, sf::Vector2u windowSize) {
		if (waveNum < 0 || waveNum >= 10) return;

		currentWave = waveNum;
		enemies.clear();

		int rows = 2;
		int cols = 3;

		
		float spacingX = windowSize.x * 0.1f;
		float spacingY = windowSize.y * 0.1f;

		float gridWidth = (cols - 1) * spacingX;
		float gridHeight = (rows - 1) * spacingY;

		
		float scale = std::min(windowSize.x, windowSize.y) / 1000.0f;

		
		sf::Vector2u textureSize = basicEnemyTexture.getSize();
		float scaledWidth = textureSize.x * scale;
		float scaledHeight = textureSize.y * scale;

		float startX = (windowSize.x - gridWidth) / 2.0f - scaledWidth / 2.0f;
		//float startY = (windowSize.y - gridHeight) / 2.0f - scaledHeight / 2.0f;
		float startY = windowSize.y * 0.1f;

		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				if (patterns[waveNum][row][col] == 1) {
					Enemy enemy(basicEnemyTexture);
					enemy.setPosition(sf::Vector2f(startX + col * spacingX, startY + row * spacingY));
					enemy.setScale(sf::Vector2f(scale, scale));
					enemies.push_back(enemy);
				}
			}
		}
	}

	void updateLayout(sf::Vector2u windowSize) {
		if (currentWave < 0) return;
		spawnWave(currentWave, windowSize);
	}


private:
	int currentWave;
	std::vector<Enemy> enemies;
	sf::Texture basicEnemyTexture;
	Enemy basicEnemy;


	int patterns[10][2][3] = {
		{
			{1, 0, 1},
			{1, 1, 1}
		},
		{
			{1, 0, 1},
			{1, 1, 1}
		}
	};

};



/*
* Example main
#include "WaveSystem.hpp"
sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Space Defender");

	const float aspectRatio = 16.0f / 9.0f;

	sf::Texture playerTexture;
	std::cout << std::filesystem::current_path() << std::endl;
	playerTexture.loadFromFile("Ship.png");
	Player player(playerTexture);

	sf::Texture enemyTexture;
	enemyTexture.loadFromFile("Enemy.png");
	Enemy enemy1(enemyTexture);


	WaveSystem waveSystem(enemyTexture);
	waveSystem.spawnWave(0, window.getSize());

	sf::Clock clock;
	while (window.isOpen())
	{
		// SFML 3-style event polling returns std::optional<sf::Event>
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			if (const auto resized = event->getIf<sf::Event::Resized>()) {
				unsigned int width = resized->size.x;
				unsigned int height = resized->size.y;

				unsigned int newHeight = static_cast<int>(width / aspectRatio);

				window.setSize({width, newHeight});

				window.setView(sf::View(sf::FloatRect({ 0.0f, 0.0f }, { static_cast<float>(width),  static_cast<float>(newHeight) })));
				waveSystem.updateLayout(window.getSize());
			}
		}

		float dt = clock.restart().asSeconds();
		player.update(dt, window);


		window.clear();
		window.draw(player);
		for(const auto& enemy : waveSystem.getEnemies()) {
			window.draw(enemy);
		}
		window.display();
	}
	


*/