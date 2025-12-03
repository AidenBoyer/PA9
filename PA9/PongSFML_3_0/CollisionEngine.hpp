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

//// example main
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Space Defender");
//
//    const float aspectRatio = 16.0f / 9.0f;
//
//    sf::Texture playerTexture;
//    playerTexture.loadFromFile("Ship.png");
//    sf::Texture playerBulletTexture;
//    playerBulletTexture.loadFromFile("Bullet.png");
//    Player player(playerTexture, playerBulletTexture, 500);
//
//    sf::Texture enemyTexture;
//    enemyTexture.loadFromFile("Enemy.png");
//    Enemy enemy1(enemyTexture);
//
//    std::list<Bullet> playerBullets;
//
//    sf::Clock clock;
//    window.setFramerateLimit(200);
//    while (window.isOpen())
//    {
//        // SFML 3-style event polling returns std::optional<sf::Event>
//        while (auto event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//                window.close();
//
//            if (const auto resized = event->getIf<sf::Event::Resized>()) {
//                unsigned int width = resized->size.x;
//                unsigned int height = resized->size.y;
//
//                unsigned int newHeight = static_cast<int>(width / aspectRatio);
//
//                window.setSize({ width, newHeight });
//
//                window.setView(sf::View(sf::FloatRect({ 0.0f, 0.0f }, { static_cast<float>(width),  static_cast<float>(newHeight) })));
//            }
//        }
//
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
//            playerBullets.push_back(player.fire());
//        }
//
//        float dt = clock.restart().asSeconds();
//
//        //update bullets (remove if no longer alive)
//        for (auto it = playerBullets.begin(); it != playerBullets.end();) {
//            if (!it->isAlive()) {
//                it = playerBullets.erase(it);
//            }
//            else {
//                it->update(dt);
//                ++it;
//            }
//        }
//        if (player.isAlive()) {
//            player.update(dt, window);
//        }
//
//
//
//        window.clear();
//        for (auto& bullet : playerBullets) {
//            window.draw(bullet);
//        }
//        window.draw(player);
//        window.draw(enemy1);
//        window.display();
//    }
//}