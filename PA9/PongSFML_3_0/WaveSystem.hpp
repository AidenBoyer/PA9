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

		// Scale spacing relative to window size
		float spacingX = windowSize.x * 0.1f;
		float spacingY = windowSize.y * 0.1f;

		float gridWidth = (cols - 1) * spacingX;
		float gridHeight = (rows - 1) * spacingY;

		// Calculate scale based on window size
		float scale = std::min(windowSize.x, windowSize.y) / 600.0f; // Relative to base size of 600

		// Get the scaled sprite dimensions to properly center
		sf::Vector2u textureSize = basicEnemyTexture.getSize();
		float scaledWidth = textureSize.x * scale;
		float scaledHeight = textureSize.y * scale;

		float startX = (windowSize.x - gridWidth) / 2.0f - scaledWidth / 2.0f;
		float startY = (windowSize.y - gridHeight) / 2.0f - scaledHeight / 2.0f;

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


	waveSystem.updateLayout(window.getSize()); // Reposition and rescale enemies


*/