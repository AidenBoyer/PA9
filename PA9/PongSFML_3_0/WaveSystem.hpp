#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"
#include <random>
class WaveSystem {

public:
	WaveSystem(const sf::Texture& baiscTexture)
		:basicEnemyTexture(baiscTexture), basicEnemy(basicEnemyTexture),
		currentWave(-1) {

	}
	std::vector<Enemy>& getEnemies() {
		return enemies;
	}
	void spawnWave(int waveNum, sf::Vector2u windowSize) {
		if (waveNum < 0 || waveNum >= 2) return;

		currentWave = waveNum;
		this->windowSize = windowSize;
		enemies.clear();

		
		moveSpeed = windowSize.x * 0.1f;
		maxMoveDistance = windowSize.x * 0.15f;
		moveDistance = 0.0f;
		moveDirection = 1.0f;

		


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

	void updateLayout(sf::Vector2u newWindowSize) {
		if (currentWave < 0 || enemies.empty()) return;


		

		// Old layout
		float oldSpacingX = windowSize.x * 0.1f;
		float oldSpacingY = windowSize.y * 0.1f;
		float oldGridWidth = (cols - 1) * oldSpacingX;
		float oldScale = std::min(windowSize.x, windowSize.y) / 1000.0f;
		sf::Vector2u textureSize = basicEnemyTexture.getSize();
		float oldScaledWidth = textureSize.x * oldScale;
		float oldStartX = (windowSize.x - oldGridWidth) / 2.0f - oldScaledWidth / 2.0f;
		float oldStartY = windowSize.y * 0.1f;

		// New layout
		float newSpacingX = newWindowSize.x * 0.1f;
		float newSpacingY = newWindowSize.y * 0.1f;
		float newGridWidth = (cols - 1) * newSpacingX;
		float newScale = std::min(newWindowSize.x, newWindowSize.y) / 1000.0f;
		float newScaledWidth = textureSize.x * newScale;
		float newStartX = (newWindowSize.x - newGridWidth) / 2.0f - newScaledWidth / 2.0f;
		float newStartY = newWindowSize.y * 0.1f;

		// Reposition and rescale existing enemies
		int enemyIndex = 0;
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				if (patterns[currentWave][row][col] == 1) {
					if (enemyIndex < enemies.size()) {
						enemies[enemyIndex].setPosition(sf::Vector2f(
							newStartX + col * newSpacingX,
							newStartY + row * newSpacingY
						));
						enemies[enemyIndex].setScale(sf::Vector2f(newScale, newScale));
						enemyIndex++;
					}
				}
			}
		}

		// Update window size and recalculate movement parameters
		windowSize = newWindowSize;
		moveSpeed = windowSize.x * 0.1f;
		maxMoveDistance = windowSize.x * 0.15f;
	}

	void updateEnemies(float dt) {
		if (enemies.empty()) {
			static std::random_device rd;
			static std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 1);
			int randomWave = dis(gen);
			spawnWave(randomWave, windowSize);
			return;
		}

		float movement = moveDirection * moveSpeed * dt;

		for (auto& enemy : enemies) {
			enemy.move(sf::Vector2f(movement, 0.0f));
		}

		moveDistance += std::abs(movement);

		if (moveDistance >= maxMoveDistance) {
			moveDirection *= -1.0f;
			moveDistance = 0.0f;
		}
	}

private:
	int rows = 2;
	int cols = 3;
	int currentWave;
	std::vector<Enemy> enemies;
	sf::Texture basicEnemyTexture;
	Enemy basicEnemy;

	sf::Vector2u windowSize;
	float moveDirection = 1.0f;
	float moveSpeed = 0.0f;
	float moveDistance = 0.0f;
	float maxMoveDistance = 0.0f;

	// [ammount of waves][rows][cols]
	int patterns[2][2][3] = {
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