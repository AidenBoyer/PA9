#pragma once
#include <SFML/Graphics.hpp>

<<<<<<< HEAD
class Enemy : public sf::Sprite 
{

public:
	Enemy(const sf::Texture& texture)
		: sf::Sprite(texture)
	{

	}

	void propagateEnemy()
	{

	}

private:
	//assign HP to enemy
	int health = 50;


};
=======
class Enemy : public sf::Sprite {
public:
    Enemy(const sf::Texture& texture, double speed = 240, double health = 100)
        : sf::Sprite(texture), enemySpeed(speed), enemyHealth(health)
    {
    }

    void update(float dt) {
        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += 1.f;

        // Normalize movement vector for diagonal movement
        if (movement.x != 0.f || movement.y != 0.f) {
            float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= len;
        }

        move(movement * enemySpeed * dt);

    }

private:
    float enemySpeed;
    float enemyHealth;
};
>>>>>>> main
