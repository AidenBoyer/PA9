#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite {
public:
    Enemy(const sf::Texture& texture, double speed = 240, double health = 100)
        : sf::Sprite(texture), enemySpeed(speed), enemyHealth(health)
    {
    }

    void update(float dt) {
        sf::Vector2f movement(0.f, 0.f);

        // Normalize movement vector for diagonal movement
        if (movement.x != 0.f || movement.y != 0.f) {
            float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= len;
        }

        move(movement * enemySpeed * dt);

    }

    void dealDamage(double damage) {
        enemyHealth -= damage;
        if (enemyHealth < 0) {
            enemyHealth = 0;
            alive = false;
        }

    }

    void destroy() { alive = false; }
    bool isAlive() const { return alive; }

private:
    bool alive = true;
    float enemySpeed = 240;
    float enemyHealth = 100;
    
};