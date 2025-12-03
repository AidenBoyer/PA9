#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.hpp"



class Player : public sf::Sprite {
public:
    Player(const sf::Texture& texture, const sf::Texture& bulletTexture, double speed = 240)
        : sf::Sprite(texture), bulletTexture(bulletTexture), playerSpeed(speed){}
    void update(float dt, const sf::RenderWindow& window) {
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
        Bullet bullet(bulletTexture);
        bullet.setPosition(this->getPosition());
        bullet.setDirection(sf::Vector2f(0, -1));
        return bullet;
    }

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
    sf::Vector2u windowSize;
    float playerSpeed = 240.f;
    bool alive = true;
    sf::Texture bulletTexture;
};