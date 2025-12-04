#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.hpp"



class Player : public sf::Sprite {
public:
    Player(const sf::Texture& texture, Bullet& masterBullet, double speed = 400, double fireCooldown = 0.5)
        : sf::Sprite(texture), masterBullet(&masterBullet), playerSpeed(speed), maxFireCooldown(fireCooldown) {}
    void update(float dt, const sf::RenderWindow& window) {

        // update firing cooldown
        fireCooldown -= dt;
        if (fireCooldown < 0.0) fireCooldown = 0.0;

        // movement
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

        float scaledSpeed = playerSpeed * std::min(windowSize.x, windowSize.y) / 1000.0f;
        move(movement * scaledSpeed * dt);

        // Clamp the player inside the window bounds
        auto bounds = getGlobalBounds();
        sf::Vector2u winSize = window.getSize();

        float minX = 0.f;
        float minY = 0.f;
        float maxX = static_cast<float>(winSize.x) - bounds.size.x;
        float maxY = static_cast<float>(winSize.y) - bounds.size.y;

        sf::Vector2f pos = getPosition();
        // Ensure position is within [min, max]
        pos.x = std::clamp(pos.x, minX, maxX);
        pos.y = std::clamp(pos.y, minY, maxY);
        setPosition(pos);
    }
    //void update(float dt) {
    //    sf::Vector2f movement(0.f, 0.f);
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    //        movement.y -= 1.f;
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    //        movement.x -= 1.f;
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    //        movement.y += 1.f;
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    //        movement.x += 1.f;
    //    // Normalize movement vector for diagonal movement
    //    if (movement.x != 0.f || movement.y != 0.f) {
    //        float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    //        movement /= len;
    //    }
    //    move(movement * playerSpeed * dt
    //}

    Bullet fire() {
        Bullet bullet(*masterBullet);
        
        sf::Vector2f playerCenter(getPosition() + sf::Vector2f((getGlobalBounds().size.x / 2), (getGlobalBounds().size.y / 2)));
        sf::Vector2f bulletPositionOffest(sf::Vector2f((bullet.getGlobalBounds().size.x / 2), (bullet.getGlobalBounds().size.y / 2)));
        bullet.setPosition(playerCenter - bulletPositionOffest);

        bullet.setDirection(sf::Vector2f(0, -1));
        fireCooldown = maxFireCooldown;
        return bullet;
    }

    double getFireCooldown() { return fireCooldown; }

    void destroy() { alive = false; }
    bool isAlive() const { return alive; }

    void initialize(sf::Vector2u windowSize) {
        updateLayout(windowSize);
    }

    void updateLayout(sf::Vector2u windowSize) {
        this->windowSize = windowSize;

        float scale = std::min(windowSize.x, windowSize.y) / 1000.0f;
        setScale(sf::Vector2f(scale, scale));

        auto bounds = getGlobalBounds();
        float x = (static_cast<float>(windowSize.x) - bounds.size.x) / 2.0f;
        float y = static_cast<float>(windowSize.y) - bounds.size.y - (windowSize.y * 0.05f);

        setPosition(sf::Vector2f(x, y));
    }


private:
    Bullet* masterBullet;
    sf::Vector2u windowSize;
    float playerSpeed = 400.f;
    double fireCooldown = 0.0;
    double maxFireCooldown = 1.0;
    bool alive = true;
};