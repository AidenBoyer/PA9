#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Enemy : public sf::Sprite {
public:
    Enemy(const sf::Texture& texture, double speed = 240, double health = 100, double firingCooldownMin = 1.0, double firingCooldownMax = 6.0)
        : sf::Sprite(texture), enemySpeed(speed), enemyHealth(health), fireCooldownMin(firingCooldownMin), fireCooldownMax(firingCooldownMax)
    {
        setRandomCooldown();
    }

    void setRandomCooldown() {
        int min1000 = fireCooldownMin * 1000;
        int max1000 = fireCooldownMax * 1000;
        fireCooldown = ((rand() % (max1000 - min1000)) + min1000) / 1000.0;
    }

    void update(float dt) {

        // update firing cooldown
        reduceFireCooldown(dt);

        // movement
        sf::Vector2f movement(0.f, 0.f);

        // Normalize movement vector for diagonal movement
        if (movement.x != 0.f || movement.y != 0.f) {
            float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= len;
        }

        move(movement * enemySpeed * dt);

    }

    void reduceFireCooldown(float dt) {
        fireCooldown -= dt;
        if (fireCooldown <= 0.0) fireCooldown = 0.0;
    }

    double getFireCooldown() { return fireCooldown; }

    Bullet fire(Bullet& masterBullet) {
        Bullet bullet(masterBullet);

        sf::Vector2f playerCenter(getPosition() + sf::Vector2f((getGlobalBounds().size.x / 2), (getGlobalBounds().size.y / 2)));
        sf::Vector2f bulletPositionOffest(sf::Vector2f((bullet.getGlobalBounds().size.x / 2), (bullet.getGlobalBounds().size.y / 2)));
        bullet.setPosition(playerCenter - bulletPositionOffest);

        bullet.setDirection(sf::Vector2f(0, 1));
        setRandomCooldown();
        return bullet;
    }

    void dealDamage(double damage) {
        enemyHealth -= damage;
        if (enemyHealth <= 0) {
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
    double fireCooldown;
    double fireCooldownMin;
    double fireCooldownMax;
    
};