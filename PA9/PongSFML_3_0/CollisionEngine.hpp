#pragma once

#include <SFML/graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
using std::vector;
using std::shared_ptr;

class CollisionEngine {
public:
	void calculateCollisions(
		vector<Player>& players,
		vector<Bullet>& playerBullets,
		vector<Enemy>&  enemies,
		vector<Bullet>& enemyBullets/*, vector<PowerUp>& powerUps*/) {
		// check player collisions
		for (auto &player : players) {
			//check for collision with enemy bullets
			for (auto &enemyBullet : enemyBullets) {
				if (checkCollision(player, enemyBullet)) {
					player.destroy();
				}
			}
			// check for collision with enemies
			for (auto &enemy : enemies) {
				if (checkCollision(player, enemy)) {
					player.destroy();
				}
			}

			//// check for power up collision
			//for (auto &powerUp : powerUps) {
			//	if (checkCollision(player, powerUp)) {
			//		// TODO: player.applyAffect(powerUp);
			//		// TODO: powerUp.destory();
			//	}
			//}
		}

		// check enemy collisions
		for (auto &enemy : enemies) {
			//check for collision with player bullets
			for (auto &playerBullet : playerBullets) {
				if (checkCollision(enemy, playerBullet)) {
					enemy.dealDamage(playerBullet.getDamage());
				}
			}
		}

		// check player bullet on enemy bullet collisions
		for (auto &playerBullet : playerBullets) {
			for (auto &enemyBullet : enemyBullets) {
				if (checkCollision(playerBullet, enemyBullet)) {
					playerBullet.destroy();
					enemyBullet.destroy();
				}
			}
		}
	}
	
	bool checkCollision(sf::Sprite &obj1, sf::Sprite &obj2) {
		return obj1.getGlobalBounds().findIntersection(obj2.getGlobalBounds()).has_value();
	}
};

